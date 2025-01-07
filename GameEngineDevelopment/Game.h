#pragma once
//#include <SDL.h>
#include <stdio.h>
#include <string>
//#include <SDL_ttf.h>
#include <memory>

#include "EventMarker.h"
#include "GameModeI.h"

class Bitmap;
class Input;
class sceneHierarchy;

class TextureManager;

class GameObject;
class SpriteRenderer;

class EditorUI;

//struct TTF_Font;
//class SDL_Color;
struct SDL_Window;
struct SDL_Renderer;

/// @brief This is the main class that will hold all the data relating to the game engine and editor
class Game
{
private:
	Input* input;
	SDL_Window* m_window;
	SDL_Renderer* gameRenderer;

	EditorUI* editorUI;

	sceneHierarchy* activeScene;

	GameModel::GameMode currentMode = GameModel::GameMode::GM_Editor;

	unsigned int frameRate = 60.0;

	bool gameRunning = true;

	const std::string sceneSaveLocation = "../sceneData/";
	std::string bufferedSceneName = "";
	bool sceneHasBeenBuffered = false;

	/// @brief This will happen at the end of the scene so that a scene isnt chaged halfway through an update loop
	/// @param ptrToSceneToChange - a referecne to the ptr to the active scene that will be changed
	/// @param rendererToUse - a pointer the the gameRenderer, so it know what renderer to build gameObject and componets for
	/// @return - returns a bool, true if successfully loaded, false if failed
	bool loadBufferedScene(sceneHierarchy*& ptrToSceneToChange, SDL_Renderer* rendererToUse);
public:
	/// @brief This will create and Game that will house an empty scene and all the functionality of the game engine and editor
	/// @param frameRate - the disired frame rate of the game - defaults to 60
	Game(int frameRate = 60);
	~Game();

	/// @brief This changes the background colour of the game
	/// @param red - red value 0-255
	/// @param blue - blue value 0-255
	/// @param green - green value 0-255
	/// @param alpha - alpha value 0-255
	void SetDisplayColour(int red, int blue, int green, int alpha);

	/// @brief This will update everything in the game engine and editor
	void Update(void);

	/// @return bool - true if the game is running/active or false if the game in notrunning/inactive
	bool GetGameRunning();

	/// @brief saves the current scene to a file
	void saveActiveScene();

	/// @brief This will queue a scene to load that will be loaded at the end of the current update loop
	/// @param sceneNameToLoad - the name of the scene to load as string
	void loadActiveScene(std::string sceneNameToLoad);

	/// @brief This will switch the current mode to the opposite of what it currently is 
	void switchMode();

	/// @return GameMode - returns the current mode the game is in
	GameModel::GameMode getCurrentMode();

	/// @return std::string - returns the current mode as a string
	std::string getCurrentModeAsString();
};

