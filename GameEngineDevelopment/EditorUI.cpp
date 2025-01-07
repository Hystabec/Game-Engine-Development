#include "EditorUI.h"

#include <filesystem>
#include <SDL.h>
#include <SDL_ttf.h>
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

#include "DebugMenu.h"
#include "AssetEditor.h"
#include "EditorHierarchy.h"
#include "EditorDetailsPannel.h"
#include "EditorFlameGraph.h"
#include "Profiler.h"
#include "sceneHierarchy.h"
#include "Game.h"

#include "EventManager.h"
#include "EventListener.h"
#include "EventMarker.h"

EditorUI::EditorUI(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow, SDL_Renderer* gameRenderer, Game* owningGame)
{
	_activeScene = pointerToTheActiveScene;
	_gameWindow = gameWindow;
	_gameRenderer = gameRenderer;
	_owningGame = owningGame;

	debugMenu = new DebugMenu(_activeScene, _gameWindow);
	assetEditor = new AssetEditor(_gameWindow, _gameRenderer, _activeScene);
	detailsPannel = new EditorDetailsPannel(_gameWindow, _gameRenderer, _activeScene);
	editorHierarchy = new EditorHierarchy(_activeScene, _gameWindow, detailsPannel);
	flameGraphPannel = new EditorFlameGraph();

	SDL_GLContext _glContext = SDL_GL_CreateContext(_gameWindow);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	ImGuiSDL::Initialize(_gameRenderer, DisplayMode.w, DisplayMode.h);	
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(_gameWindow, _glContext);	

	loadFileNames("../sceneData/");

	MousePosMarker = new EventMarker(EventMarker::EVENT_MARKER_TYPE_BOOL);
	MousePosListener = new EventListener(eventType::EVENT_INPUT_LEFT_CONTROL, Event::TYPE_BOOL, MousePosMarker);

	TTF_Init();

	textfont = TTF_OpenFont("../Assets/DejaVuSans.ttf", 15);

}

EditorUI::~EditorUI()
{
	if (assetEditor)
	{
		delete assetEditor;
		assetEditor = nullptr;
	}

	if (debugMenu)
	{
		delete debugMenu;
		debugMenu = nullptr;
	}

	if (editorHierarchy)
	{
		delete editorHierarchy;
		editorHierarchy = nullptr;
	}

	if (detailsPannel)
	{
		delete detailsPannel;
		detailsPannel = nullptr;
	}

	if (flameGraphPannel)
	{
		delete flameGraphPannel;
		flameGraphPannel = nullptr;
	}

	TTF_CloseFont(textfont);
	delete MousePosListener;
}

void EditorUI::sceneNameChangeWindow(bool* isOpen)
{
	if (*isOpen == false)
		return;

	ImGui::SetNextWindowSize(ImVec2(300,100));

	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("SceneNameChange", nullptr, flags);

	static char inputBuffer[64] = "";
	static bool wasInputEmpty = false;
	char nameBuffer[32];
	sprintf_s(nameBuffer, (*_activeScene)->getSceneName().c_str());

	ImGui::Text("Enter new scene name:");
	ImGui::InputText("##inputScene", inputBuffer, 64);

	ImGui::SameLine();
	if (ImGui::Button("Enter"))
	{
		wasInputEmpty = false;

		std::string inputtedValue = inputBuffer;

		if (inputtedValue == "")
			wasInputEmpty = true;
		else
		{
			(*_activeScene)->changeSceneName(inputtedValue);
			*isOpen = false;
		}
	}

	if (wasInputEmpty)
		ImGui::Text("Name cannot be empty");

	if (ImGui::Button("Close"))
	{
		wasInputEmpty = false;
		inputBuffer[0] = '\0';
		*isOpen = false;
	}

	ImGui::End();
}

void EditorUI::loadSceneWindow(bool* isOpen)
{
	if (*isOpen == false)
		return;

	ImGui::SetNextWindowSize(ImVec2(300, 100));

	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("SceneNameChange", nullptr, flags);

	int currentIDextention = 0;
	for (std::string fileName : vectorOfFileNames)
	{
		char tempButtonID[32];
		sprintf_s(tempButtonID, "open ##%d", currentIDextention);

		ImGui::Text(fileName.c_str());
		ImGui::SameLine();
		if (ImGui::Button(tempButtonID))
		{
			_owningGame->loadActiveScene(fileName);
			*isOpen = false;
			break;	
		}

		currentIDextention++;
	}

	if (ImGui::Button("Close"))
	{
		*isOpen = false;
	}

	ImGui::End();
}

void EditorUI::loadFileNames(std::string folderPath)
{
	vectorOfFileNames.clear();

	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
	{
		if (entry.path().extension() == ".SCDAT")
		{
			vectorOfFileNames.push_back(entry.path().filename().replace_extension().string());
		}
		else if (entry.is_directory())
		{
			loadFileNames(entry.path().string());
		}
	}
}

void EditorUI::Update(GameModel::GameMode currentGameMode)
{
	static bool sceneNameChange = false;
	static bool sceneLoad = false;
	static bool showProfiler = false;
	static bool showHierchy = true;
	static bool showAssetMenu = true;

	profiler->startProfile("EditorUI Update");
	ImGui::NewFrame();
	ImGui_ImplSDL2_NewFrame(_gameWindow);

    if (std::get<bool>(MousePosMarker->getData()) == true)
	{

		int textw=0;
		int texth=0;
		int previoush = 0;

		std::string myMessageTwo = "X: " + std::to_string((int)ImGui::GetMousePos().x);
		std::string myMessageThree = "Y: " + std::to_string((int)ImGui::GetMousePos().y);
		SDL_Surface* messageSurface = TTF_RenderText_Solid(textfont, myMessageTwo.c_str(), {0,0,255});
		SDL_Texture* MessageTexture = SDL_CreateTextureFromSurface(_gameRenderer, messageSurface);
		SDL_QueryTexture(MessageTexture, NULL, NULL, &textw, &texth);

		SDL_Rect messgae_rect;
		messgae_rect.x = ImGui::GetMousePos().x + 20;
		messgae_rect.y = ImGui::GetMousePos().y + 20;
		messgae_rect.w = textw;
		messgae_rect.h = texth;
		
		previoush = texth + 20;

		SDL_RenderCopy(_gameRenderer, MessageTexture, NULL, &messgae_rect);  

		SDL_DestroyTexture(MessageTexture);
		SDL_FreeSurface(messageSurface);

		messageSurface = TTF_RenderText_Solid(textfont, myMessageThree.c_str(), {255,0,0});
		MessageTexture = SDL_CreateTextureFromSurface(_gameRenderer, messageSurface);
		SDL_QueryTexture(MessageTexture, NULL, NULL, &textw, &texth);

		messgae_rect.x = ImGui::GetMousePos().x + 20;
		messgae_rect.y = ImGui::GetMousePos().y + previoush;
		messgae_rect.w = textw;
		messgae_rect.h = texth;

		previoush = texth + previoush;

		SDL_RenderCopy(_gameRenderer, MessageTexture, NULL, &messgae_rect);

		SDL_DestroyTexture(MessageTexture);
		SDL_FreeSurface(messageSurface);
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (currentGameMode == GameModel::GameMode::GM_Editor)
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					_owningGame->saveActiveScene();
				}

				if (ImGui::MenuItem("Open"))
				{
					sceneNameChange = false;
					sceneLoad = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Change Scene Name"))
				{
					sceneLoad = false;
					sceneNameChange = true;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Toggle Hierarchy"))
				{
					showHierchy = !showHierchy;
				}

				if (ImGui::MenuItem("Toggle Asset menu"))
				{
					showAssetMenu = !showAssetMenu;
				}

				if (ImGui::MenuItem("Toggle profiler"))
				{
					showProfiler = !showProfiler;
				}

				ImGui::EndMenu();
			}
		}

		if (ImGui::BeginMenu("Play"))
		{
			std::string currentMode = "Switch to ";
			if (_owningGame->getCurrentMode() == GameModel::GameMode::GM_Editor)
				currentMode += "play";
			else if (_owningGame->getCurrentMode() == GameModel::GameMode::GM_Play)
				currentMode += "editor";

			if (ImGui::MenuItem(currentMode.c_str()))
			{
				_owningGame->switchMode();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (currentGameMode == GameModel::GameMode::GM_Editor)
	{
		sceneNameChangeWindow(&sceneNameChange);
		loadSceneWindow(&sceneLoad);
	}
	
	if (currentGameMode == GameModel::GameMode::GM_Editor)
	{
		if (showAssetMenu)
			assetEditor->Update();

		if (showHierchy)
			editorHierarchy->Update();

		detailsPannel->Update();

		if (showProfiler)
			flameGraphPannel->Update();
	}

	ImGui::EndFrame();

	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	profiler->endProfile("EditorUI Update");
}