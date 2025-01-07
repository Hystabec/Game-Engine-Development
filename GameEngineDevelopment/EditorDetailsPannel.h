#pragma once
#include <memory>
#include <vector>
#include "I_EditorPannel.h"

struct SDL_Window;
struct SDL_Renderer;
class GameObject;
class I_ComponentGUI;
class sceneHierarchy;

class EditorDetailsPannel : public I_EditorPannel
{
public:
	EditorDetailsPannel(SDL_Window* gameWindow, SDL_Renderer* gameRenderer, sceneHierarchy** activeScene);
	~EditorDetailsPannel();
	virtual void Update() override;
	void changeGameObject(std::shared_ptr<GameObject> newGameObject);
	void removeGameObject();
private:
	void popUpWindow();
	SDL_Window* _gameWindow = nullptr;
	SDL_Renderer* _gameRenderer = nullptr;
	sceneHierarchy** _activeScene = nullptr;

	std::weak_ptr<GameObject> currentGameObjectDetials;
	std::vector<I_ComponentGUI*> currentGameObjectComponets;
};