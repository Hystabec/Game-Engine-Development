#include "Game.h"
#include "bitmap.h"
#include "Input.h"

#include "Logger.h"
#include "Profiler.h"
#include "TextureManager.h"

#include <stdio.h>

#include "EventManager.h"
#include "EventListener.h"
//#include "EventMarker.h"

//this is needed for the frame rate
#include <chrono>
#include <ctime>
#include <thread>

#include "GameObject.h"
#include "MasterComponent.h"
#include "INode.h"
#include "sceneHierarchy.h"
#include "Transform.h"
#include "CollisionManager.h"

#include "EditorUI.h"
#include <SDL.h>
//#include <SDL_ttf.h>

#include <filesystem>
#include <fstream>

//this is used for the clock and frame rate of the game
namespace gameclock
{
	using clock = std::chrono::steady_clock;
	auto next_frame = clock::now();
}

bool fileIsEmpty(std::ifstream& ifstreamToCheck)
{
	//this will take in an ifstream check if the start of the file is the end of file (the file is empty)
	return ifstreamToCheck.peek() == std::ifstream::traits_type::eof();
}

std::vector<std::string> parseFileLine(std::string stringLine)
{
	std::vector<std::string> returnVector;
	std::string currentString = "";

	for (int index = 0; index < stringLine.size(); index++)
	{
		if (index == 0)
			continue;

		if (stringLine[index] == '|')
		{
			returnVector.push_back(currentString);
			break;
		}
		else if (stringLine[index] == '=')
		{
			returnVector.push_back(currentString);
			currentString.clear();
		}
		else
			currentString.push_back(stringLine[index]);
	}

	return returnVector;
}

void fileAddComponent(sceneHierarchy* sceneToChange, std::vector<std::string> parsedVector)
{
	//each compoents is hard coded so if a new componet is added it also needs to be added here (not great)
	//lots of string comparisons also not great
	//all in all this is not a great solution BUT it works

	auto tempGOptr = sceneToChange->findGameObjectInSceneWithID(std::stoi(parsedVector[0]));

	if (parsedVector[1] == "Sprite Renderer")
	{
		sceneToChange->AddSpriteRenderer(tempGOptr, parsedVector[3], parsedVector[3]);
		if (parsedVector[2] == "true")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(true);
		else if (parsedVector[2] == "false")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(false);
	}
	else if (parsedVector[1] == "Basic Movement Module")
	{
		sceneToChange->AddBasicMovementModule(tempGOptr, std::stoi(parsedVector[3]));
		if (parsedVector[2] == "true")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(true);
		else if (parsedVector[2] == "false")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(false);
	}
	else if (parsedVector[1] == "Rectangle Collider")
	{
		bool passedTriggerVal = false;
		if (parsedVector[3] == "true")
			passedTriggerVal = true;

		sceneToChange->AddRectangleCollider(tempGOptr, passedTriggerVal, parsedVector[4]);
		if (parsedVector[2] == "true")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(true);
		else if(parsedVector[2] == "false")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(false);
	}
	else if (parsedVector[1] == "TestGameComp")
	{
		sceneToChange->AddTestGameComp(tempGOptr);
		if (parsedVector[2] == "true")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(true);
		else if(parsedVector[2] == "false")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(false);
	}
	else if (parsedVector[1] == "BasicFollowAI")
	{
		sceneToChange->AddBasicFollowAi(tempGOptr, vec2(std::stoi(parsedVector[3]), std::stoi(parsedVector[4])), vec2(std::stoi(parsedVector[5]), std::stoi(parsedVector[6])), std::stoi(parsedVector[7]));
		if(parsedVector[2] == "true")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(true);
		else if (parsedVector[2] == "false")
			tempGOptr->getTotalComponentsVector()[tempGOptr->getTotalNumberOfComponents() - 1]->setActiveStatus(false);
	}
}

bool Game::loadBufferedScene(sceneHierarchy*& ptrToSceneToChange, SDL_Renderer* rendererToUse)
{
	if (!sceneHasBeenBuffered)
		return false;


	//if the file is modified it will most likly expolde and the program will break (if i had to guess vector read access violation)

	std::string fileToLoad = sceneSaveLocation + bufferedSceneName + ".SCDAT";

	//checks if the file exists
	if (!(std::filesystem::exists(fileToLoad)))
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "scene data file %s could not be found\n", fileToLoad);
		bufferedSceneName = "";
		sceneHasBeenBuffered = false;
		return false;
	}
	std::ifstream fileToRead(fileToLoad);

	//checks if the file is empty
	if (fileIsEmpty(fileToRead))
	{
		fileToRead.close();
		Debug_Logger->TypeDebugPrintF(Logger_Error, "scene data file %s is empty\n", fileToLoad);
		bufferedSceneName = "";
		sceneHasBeenBuffered = false;
		return false;
	}

	sceneHierarchy* sceneInCreation;
	std::string firstLine = "";
	std::getline(fileToRead, firstLine);

	if (firstLine[0] == '#')
	{
		std::vector<std::string> nameVec;
		nameVec = parseFileLine(firstLine);

		sceneInCreation = new sceneHierarchy(this, nameVec[0], rendererToUse, std::stoul(nameVec[1]));
	}
	else
	{
		Debug_Logger->TypeDebugPrintF(Logger_Error, "incorrect scene data format\n");
		bufferedSceneName = "";
		sceneHasBeenBuffered = false;
		return false;
	}

	std::string currentLine = "";
	std::vector<std::string> lineData;
	int currentLineIndex = 1;

	while (std::getline(fileToRead, currentLine))
	{
		switch (currentLine[0])
		{
		case '@':
			//create new gameObject
			lineData = parseFileLine(currentLine);
			sceneInCreation->CreateNewGameObject(lineData[0], std::stoi(lineData[2]), std::stoi(lineData[3]), std::stoi(lineData[1]));
			lineData.clear();
			break;
		case '>':
			//add components
			lineData = parseFileLine(currentLine);

			fileAddComponent(sceneInCreation, lineData);

			lineData.clear();
			break;
		case '~':
			lineData = parseFileLine(currentLine);
			sceneInCreation->findGameObjectInSceneWithID(std::stoi(lineData[0]))->setParent(sceneInCreation->findGameObjectInSceneWithID(std::stoi(lineData[1])));
			lineData.clear();
			break;
		default:
			break;
		}

		currentLineIndex++;
	}

	sceneInCreation->initScene();

	//delete the original scene so hanging memory isnt left
	delete ptrToSceneToChange;
	ptrToSceneToChange = nullptr;

	ptrToSceneToChange = sceneInCreation;
	bufferedSceneName = "";
	sceneHasBeenBuffered = false;
	return true;
}

Game::Game(int frameRate)
{
	m_window = nullptr;
	gameRenderer = nullptr;
	gameRunning = true;

	input = new Input();

	SDL_Init(SDL_INIT_VIDEO);

	m_window = SDL_CreateWindow
	(
		"My Window",		//title of window
		250,				//initial x position
		50,					//initial y position
		1440,				//width, in pixels
		880,				//height, in pixels
		0					//window behaviour flag
	);				

	if (!m_window)
	{
		printf("WINDOW initialisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}

	gameRenderer = SDL_CreateRenderer
	(
		m_window,				//link the renderer to window
		-1,						//index rendering driver
		0						//renderer behaviour flags
	);

	if (!gameRenderer)
	{
		printf("RENDERER initialisation failed: %s\n", SDL_GetError());
		printf("Press any key to continue\n");
		getchar();
		return;
	}	

	

	activeScene = new sceneHierarchy(this, "DefaultScene", gameRenderer);
	activeScene->initScene(); //this intialized all GameObjects in the scene so this needs to be done for all the scenes after everything is in the scene

	editorUI = new EditorUI(&activeScene, m_window, gameRenderer, this);

	this->frameRate = frameRate;
	//profiler->setActive(false);
}

Game::~Game()
{
	//saveActiveScene();

	if (input)
	{
		delete input;
		input = nullptr;
	}

	if (gameRenderer)
	{
		SDL_DestroyRenderer(gameRenderer);
	}

	if (m_window)
	{
		SDL_DestroyWindow(m_window);
	}

	if (editorUI)
	{
		delete editorUI;
	}
	

	Texture_Manager->~TextureManager();
	Event_Manager->~EventManager();
	profiler->~Profiler();
}

void Game::SetDisplayColour(int red, int green, int blue, int alpha)
{
	if (gameRenderer)
	{
		int result = SDL_SetRenderDrawColor
		(
			gameRenderer,		//our target renderer
			red, 			//red
			green,			//green
			blue, 			//blue
			alpha			//alpha
		);
	}
}

void Game::Update(void)
{
	if (!input->windowCloseEventCalled())	//could ask the input manager for this instead of checking every time
	{
		if (!activeScene == 0)
		{
			profiler->startProfile("Main Update");

			gameclock::next_frame += std::chrono::milliseconds(1000 / frameRate); //get the frame rate in and calculates how long until the next frame

			if (!activeScene->getInitializedStatus())	//might be inefictant as this is checked on everyloop is here incase the active scene changes and needs to be initialized
			{
				activeScene->initScene();
			}

			//
			//clear screen
			//
			SDL_RenderClear(activeScene->getPointerToRenderer());

			//
			//update logic
			//
			input->Update();	//updates any inputs

			if (currentMode == GameModel::GameMode::GM_Play)
			{
				Collision_Manager->update();
				activeScene->updateScene();	//updates all the logical components
			}
			//
			//draw
			//			

			activeScene->drawScene();	//updates all the graphical components

		
			editorUI->Update(currentMode);	//updates the editor ui

			//
			//present
			//
			
			//sets a default background colour
			//SetDisplayColour(110, 2, 23, 255);
			SetDisplayColour(120, 120, 120, 255);

			SDL_RenderPresent(activeScene->getPointerToRenderer());

			activeScene->cleanUpScene();	//this will delete all the gameobjects in the deletion vector

			profiler->endProfile("Main Update");

			profiler->nextFrame();
			
			loadBufferedScene(activeScene, gameRenderer);

			std::this_thread::sleep_until(gameclock::next_frame);	//will wait until till the rest of the time is taken up
		}
		else
		{
			Debug_Logger->TypeDebugPrintF(Logger_Warning, "No active Scene - Game closing\n");
			gameRunning = false;
		}
	}
	else
	{
		gameRunning = false;
	}
}

bool Game::GetGameRunning()
{
	return gameRunning;
}

void Game::saveActiveScene()
{
	activeScene->saveCurrentSceneBackEnd(sceneSaveLocation);
	editorUI->loadFileNames("../sceneData/");
}

void Game::loadActiveScene(std::string sceneNameToLoad)
{
	bufferedSceneName = sceneNameToLoad;
	sceneHasBeenBuffered = true;
}

void Game::switchMode()
{
	if (currentMode == GameModel::GameMode::GM_Editor)
		currentMode = GameModel::GameMode::GM_Play;
	else if (currentMode == GameModel::GameMode::GM_Play)
		currentMode = GameModel::GameMode::GM_Editor;
}

GameModel::GameMode Game::getCurrentMode()
{
	return currentMode;
}

std::string Game::getCurrentModeAsString()
{
	std::string returnString;

	if (currentMode == GameModel::GameMode::GM_Editor)
		returnString = "Editor";
	else if (currentMode == GameModel::GameMode::GM_Play)
		returnString = "Play";

	return returnString;
}
