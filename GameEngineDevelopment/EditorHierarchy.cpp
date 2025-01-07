#include "EditorHierarchy.h"

#include "Logger.h"
#include <memory>
#include <SDL.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

#include "EditorDetailsPannel.h"
#include "sceneHierarchy.h"
#include "GameObject.h"

#include "Profiler.h"

void EditorHierarchy::popupMenu(std::shared_ptr<GameObject> GO)
{
	std::string popupName = std::to_string(GO->getGameObjectID()) + "-popUp";
	if (ImGui::BeginPopupContextItem(popupName.c_str()))
	{
		if (ImGui::Selectable("Details"))
		{
			_detailsPannel->changeGameObject(GO);
		}


		if (ImGui::Selectable("Remove parent"))
			GO->removeParent();

		if (ImGui::Selectable("Delete Game Object"))
		{
			(*_activeScene)->deleteGameObject(GO);
		}

		ImGui::EndPopup();
	}

	ImGui::OpenPopupOnItemClick(popupName.c_str(), ImGuiPopupFlags_MouseButtonRight);
}

EditorHierarchy::EditorHierarchy(sceneHierarchy** pointerToTheActiveScene, SDL_Window* gameWindow, EditorDetailsPannel* editorDetailsPannel)
{
	_activeScene = pointerToTheActiveScene;
	_gameWindow = gameWindow;
	_detailsPannel = editorDetailsPannel;
}

EditorHierarchy::~EditorHierarchy()
{
	_activeScene = nullptr;
	_gameWindow = nullptr;
}

void EditorHierarchy::recursiveDisplayChildren(std::shared_ptr<GameObject> currentGO, ImGuiTreeNodeFlags treeNodeFlags, bool* hasParentedInLoop)
{
	if (currentGO->getChildrenVector().size() < 0)
		return;

	for (std::shared_ptr<GameObject> childGO : currentGO->getChildrenVector())
	{
		bool isNodeOpen = ImGui::TreeNodeEx(std::to_string(childGO->getGameObjectID()).c_str(), treeNodeFlags, childGO->getGameObjectName().c_str());

		//repeated from main update start
		if (ImGui::IsItemClicked())
		{
			selectedGameObject = childGO;
			_detailsPannel->changeGameObject(childGO);
		}

		if (selectedGameObject.lock() == childGO && ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("_TREENODE", &childGO, sizeof(std::shared_ptr<GameObject>));
			//^^^^ this will get the pointer of the shr_ptr but the will read a size of a shared pointer
			ImGui::Text(childGO->getGameObjectName().c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
			{
				//selectedGameObject.
				std::shared_ptr<GameObject>* payloadAsGameObject = static_cast<std::shared_ptr<GameObject>*>(payload->Data);

				//will need to get target tree node deref and make payload a child of the tree node gameObject
				//payloadAsGameObject->get()->setParent(childGO);
				childGO->AddChild(*payloadAsGameObject);
				*hasParentedInLoop = true;
			}
			ImGui::EndDragDropTarget();
		}
		//repeated from main update end
		popupMenu(childGO);

		if (isNodeOpen)
		{
			recursiveDisplayChildren(childGO, treeNodeFlags, hasParentedInLoop);

			ImGui::TreePop();
		}
	}
}

void EditorHierarchy::Update()
{
	profiler->startProfile("EHierarchyUpdate");

	bool hasParentedThisLoop = false;

	std::string displaySceneName = "Scene Hirarcy - " + (*_activeScene)->getSceneName();

	ImGui::SetNextWindowPos(ImVec2(5, 25));
	ImGui::SetNextWindowSize(ImVec2(250, 600));
	ImGui::Begin(displaySceneName.c_str(), 0, ImGuiWindowFlags_NoMove);
	//ImGui::Begin(displaySceneName.c_str(), 0);

	if (ImGui::BeginPopup("RightClickMenu"))
	{
		if (ImGui::Button("Create empty gameObject"))
		{
			static int currentIndex = 0; 

			std::string stringIndex = "";
			if (currentIndex != 0)
				stringIndex = std::to_string(currentIndex);

			currentIndex++;

			std::shared_ptr<GameObject> tempGO = (*_activeScene)->CreateNewGameObject("EmptyObject" + stringIndex, 0, 0);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("RightClickMenu");
	}

	ImGuiTreeNodeFlags nodeFlgas = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	for(std::shared_ptr<GameObject> GO : (*_activeScene)->getHierarchy())
	{
		if (GO->getIsRootNode())
		{
			bool isNodeOpen = ImGui::TreeNodeEx(std::to_string(GO->getGameObjectID()).c_str(), nodeFlgas, GO->getGameObjectName().c_str());

			//drag and drop for parenting
			if (ImGui::IsItemClicked())
			{
				selectedGameObject = GO;
				_detailsPannel->changeGameObject(GO);
			}

			if (selectedGameObject.lock() == GO && ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("_TREENODE", &GO, sizeof(std::shared_ptr<GameObject>));
				//^^^^ this will get the pointer of the shr_ptr but the will read a size of a shared pointer
				ImGui::Text(GO->getGameObjectName().c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
				{
					//selectedGameObject.
					std::shared_ptr<GameObject>* payloadAsGameObject = static_cast< std::shared_ptr<GameObject>* >(payload->Data);

					//will need to get target tree node deref and make payload a child of the tree node gameObject
					GO->AddChild(*payloadAsGameObject);
					hasParentedThisLoop = true;
				}
				ImGui::EndDragDropTarget();
			}

			popupMenu(GO);

			//if the node is open then it need to show its children
			if(isNodeOpen)
			{
				recursiveDisplayChildren(GO, nodeFlgas, &hasParentedThisLoop);
				//while this happens it can no longer be drag and dropped

				ImGui::TreePop();
			}
		}		
	}

	if (hasParentedThisLoop)
		selectedGameObject.lock() = nullptr;

	ImGui::End();
	profiler->endProfile("EHierarchyUpdate");
}

