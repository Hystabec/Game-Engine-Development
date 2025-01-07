#pragma once
#include <memory>
#include "I_EditorPannel.h"
class GameObject;
class sceneHierarchy;
struct SDL_Window;
struct SDL_Renderer;
typedef int ImGuiTreeNodeFlags;
class EditorDetailsPannel;


class EditorHierarchy : public I_EditorPannel
{
public:
	EditorHierarchy(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow, EditorDetailsPannel* editorDetailsPannel);
	~EditorHierarchy();
	virtual void Update() override;

private:
	void recursiveDisplayChildren(std::shared_ptr<GameObject> currentGO, ImGuiTreeNodeFlags treeNodeFlags, bool* hasParentedInLoop);
	void popupMenu(std::shared_ptr<GameObject> GO);
	sceneHierarchy** _activeScene;
	SDL_Window* _gameWindow;
	EditorDetailsPannel* _detailsPannel;

	std::weak_ptr<GameObject> selectedGameObject;
};