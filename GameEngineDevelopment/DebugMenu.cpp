#include "DebugMenu.h"

#include <stdio.h>
#include <typeinfo>

#include "EventManager.h"
#include "EventListener.h"
#include "EventMarker.h"

#include "sceneHierarchy.h"
#include "GameObject.h"
#include "Transform.h"
#include "MasterComponent.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>
#include <SDL.h>

DebugMenu::DebugMenu(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow)
{
	_activeScene = pointerToTheActiveScene;
	_gameWindow = gameWindow;

	DebugEventMarker = new EventMarker(EventMarker::EVENT_MARKER_TYPE_BOOL);
	DebugEventListener = new EventListener(eventType::EVENT_INPUT_GRAVE, Event::TYPE_BOOL, DebugEventMarker);
}

DebugMenu::~DebugMenu()
{
	_activeScene = nullptr;
	_gameWindow = nullptr;
	delete DebugEventListener;
}

void DebugMenu::Update()
{
	if (std::get<bool>(DebugEventMarker->getData()) == true && std::get<bool>(DebugEventMarker->getData()) != previousDebugMenuActiveBool)
	{
		if (ToggleDebugMenuBool == true)
			ToggleDebugMenuBool = false;
		else
			ToggleDebugMenuBool = true;
	}
	previousDebugMenuActiveBool = std::get<bool>(DebugEventMarker->getData());

	if (ToggleDebugMenuBool)
	{
		DebugScreen(nullptr);
	}
}

void displayGameObjectData(std::shared_ptr<GameObject> GO)
{
	ImGui::Text("Active Status: %s", GO->getActiveStatus() ? "active" : "inactive");
	ImGui::Text("Transform - position");
	ImGui::Text("X: %d", GO->getTransform().Position().x);
	ImGui::Text("Y: %d", GO->getTransform().Position().y);
	ImGui::Text("Local Transform - position");
	ImGui::Text("X: %d", GO->getLocalTransform().Position().x);
	ImGui::Text("Y: %d", GO->getLocalTransform().Position().y);
}


void DebugMenu::DebugScreen(bool* displayDebug)
{
	static bool no_resize = true;
	static bool no_titlebar = true;
	static bool no_scrollbar = true;
	static bool no_background = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_resize)		window_flags |= ImGuiWindowFlags_NoResize;
	if (no_titlebar)	window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)	window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_background)	window_flags |= ImGuiWindowFlags_NoBackground;

	const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + 50, mainViewport->WorkPos.y + 50), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_None);

	if (!ImGui::Begin("DBWindow", displayDebug, window_flags))
	{
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGui::Text("Active scene - %s", (*_activeScene)->getSceneName().c_str());

	static int CurrentGameObjectIdx = 0;
	if (ImGui::BeginListBox("GameObjects", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int tempGameObjectIdx = 0; tempGameObjectIdx < (*_activeScene)->getHierarchy().size(); tempGameObjectIdx++)
		{
			const bool isSelected = (CurrentGameObjectIdx == tempGameObjectIdx);
			if (ImGui::Selectable((*_activeScene)->getHierarchy()[tempGameObjectIdx]->getGameObjectName().c_str(), isSelected))
				CurrentGameObjectIdx = tempGameObjectIdx;

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
	
	displayGameObjectData((*_activeScene)->getHierarchy()[CurrentGameObjectIdx]);

	if ((*_activeScene)->getHierarchy()[CurrentGameObjectIdx]->getTotalNumberOfComponents() > 0)
	{
		ImGui::Text(" ");
		ImGui::Text("Components: ");
		for (int tempComponentIdx = 0; tempComponentIdx < (*_activeScene)->getHierarchy()[CurrentGameObjectIdx]->getTotalNumberOfComponents(); tempComponentIdx++)
		{
			//this will loop through all the logic components

			ImGui::Text
			(
				"%s - %s",
				(*_activeScene)->getHierarchy()[CurrentGameObjectIdx]->getTotalComponentsVector()[tempComponentIdx]->getComponentName().c_str(),
				(*_activeScene)->getHierarchy()[CurrentGameObjectIdx]->getTotalComponentsVector()[tempComponentIdx]->getActiveStatus()?"active":"inactive"
			);

			if(BasicMovementModule* tempSR = dynamic_cast<BasicMovementModule*>((*_activeScene)->getHierarchy()[CurrentGameObjectIdx]->getTotalComponentsVector()[tempComponentIdx]))
			{
				ImGui::Text("...Movement Speed: %d", tempSR->getSpeedToMove());
			}
		}		
	}

	ImGui::PopItemWidth();
	ImGui::End();
}

