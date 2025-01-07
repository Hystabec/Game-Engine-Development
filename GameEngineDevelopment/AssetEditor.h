#pragma once

#include "I_EditorPannel.h"
#include <vector>
#include <string>


struct SDL_Window;
struct SDL_Renderer;

class sceneHierarchy;

class AssetEditor : public I_EditorPannel
{
public:
	AssetEditor(SDL_Window* gameWindow, SDL_Renderer* gameRenderer, sceneHierarchy** activeScene);
	~AssetEditor();
	virtual void Update() override;
private:
	unsigned int currentCreatedGameObjectIndex = 0;
	void loadAssetsInDirectory(std::string pathToFollow);
	std::vector<std::string> contentTexture;
	std::string path = "../Assets";
	std::string assetDragValue = "";
	SDL_Window* _gameWindow;
	SDL_Renderer* _gameRenderer;
	sceneHierarchy** currentScene;
};