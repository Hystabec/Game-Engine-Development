#include "AssetEditor.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

#include <SDL.h>
#include <filesystem>
#include "TextureManager.h"

#include "sceneHierarchy.h"
#include "GameObject.h"
#include "Profiler.h"

AssetEditor::AssetEditor(SDL_Window* gameWindow, SDL_Renderer* gameRenderer, sceneHierarchy** activeScene)
{
	_gameWindow = gameWindow;
	_gameRenderer = gameRenderer;
	currentScene = activeScene;

	loadAssetsInDirectory(path);
}

AssetEditor::~AssetEditor()
{
	_gameRenderer = nullptr;
	_gameWindow = nullptr;
	currentScene = nullptr;
}

void AssetEditor::Update()
{
	profiler->startProfile("EAssetUpdate");

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && assetDragValue.length() != 0)
	{
		//this will be ran if the mouse has been released while also dragging an item from the assets menu

		int x, y;
		SDL_GetMouseState(&x, &y);

		std::string stringIndex = "";

		if (currentCreatedGameObjectIndex != 0)
			stringIndex = std::to_string(currentCreatedGameObjectIndex);

		currentCreatedGameObjectIndex++;

		//this creates a new gameObject with a sprite renderer that will have the sprite of the value saved in "assetDragValue"
		std::shared_ptr<GameObject> tempGO = (*currentScene)->CreateNewGameObject("NewGameObject" + stringIndex, x, y);
		(*currentScene)->AddSpriteRenderer(tempGO, assetDragValue, assetDragValue);
		tempGO->initialize();

		//sets assetDragValue back to an empty string so that another item from the asset list needs to be grabbed before running again
		assetDragValue = "";
	}


	ImGui::Begin("Asset Editor");
	ImGui::BeginChild("Content", ImVec2(), true);

	for (int i = 0; i < contentTexture.size(); i++)
	{
		//i is none discriptive especially for ids as something else might also try to push id i 
		
		int idOffset = 1000000;
		//could do something like this so that anything that is in the id range of 1000000->1999999 will be a button id

		ImGui::PushID(idOffset+i);
		ImGui::ImageButton(Texture_Manager->getTextureRefWithLocation(contentTexture[i]), { 100,100 });

		if (ImGui::BeginDragDropSource())
		{
			std::string contentData = contentTexture[i];
			assetDragValue = contentData;
			ImGui::Image((ImTextureID)Texture_Manager->getTextureRefWithLocation(contentTexture[i]), { 100,100 });
			ImGui::EndDragDropSource();
		}

		ImGui::PopID();
		ImGui::SameLine();
	}

	ImGui::EndChild();
	ImGui::End();

	profiler->endProfile("EAssetUpdate");
}

void AssetEditor::loadAssetsInDirectory(std::string pathToFollow)
{
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == ".bmp" || entry.path().extension() == ".jpg" || entry.path().extension() == ".png")
		{
			Texture_Manager->Load(entry.path().string(), true, _gameRenderer);
			contentTexture.push_back(entry.path().string());
		}
		else if (entry.is_directory())
		{
			loadAssetsInDirectory(entry.path().string());
		}
	}
}