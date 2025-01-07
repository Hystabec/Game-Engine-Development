#pragma once
#include <vector>
#include <string>
#include "GameModeI.h"


class sceneHierarchy;
struct SDL_Window;
struct SDL_Renderer;

class DebugMenu;
class AssetEditor;
class EditorHierarchy;
class EditorDetailsPannel;
//class DebugFPS;
class EditorFlameGraph;
class I_EditorPannel;
class Game;

class EventMarker;
class EventListener;

typedef struct _TTF_Font TTF_Font;
struct SDL_Color;

class EditorUI
{
public:
	EditorUI(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow, SDL_Renderer* gameRenderer, Game* owningGame);
	~EditorUI();

	void Update(GameModel::GameMode currentGameMode);
	void loadFileNames(std::string folderPath);
private:
	sceneHierarchy** _activeScene;
	SDL_Window* _gameWindow;
	SDL_Renderer* _gameRenderer;
	DebugMenu* debugMenu;
	AssetEditor* assetEditor;
	EditorHierarchy* editorHierarchy;
	EditorDetailsPannel* detailsPannel;
	EditorFlameGraph* flameGraphPannel;
	Game* _owningGame;
	std::vector<std::string> vectorOfFileNames;
	void sceneNameChangeWindow(bool* isOpen);
	void loadSceneWindow(bool* isOpen);

	EventMarker* MousePosMarker;
	EventListener* MousePosListener;

	TTF_Font* textfont;
};