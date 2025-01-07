#pragma once
#include "I_EditorPannel.h"

class EventMarker;
class EventListener;
class sceneHierarchy;

struct SDL_Window;
//class SDL_Renderer;

//typedef void* SDL_GLContext;

class DebugMenu : public I_EditorPannel
{
public:
	DebugMenu(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow);
	~DebugMenu();

	virtual void Update() override;
private:
	void DebugScreen(bool* displayDebug);
	sceneHierarchy** _activeScene;
	SDL_Window* _gameWindow;
	//SDL_Renderer* _gameRenderer;
	//SDL_GLContext _glContext;
	EventMarker* DebugEventMarker;
	EventListener* DebugEventListener;
	bool previousDebugMenuActiveBool = false;
	bool ToggleDebugMenuBool = false;
};
