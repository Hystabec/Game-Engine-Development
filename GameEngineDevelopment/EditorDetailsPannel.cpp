#include "EditorDetailsPannel.h"

#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <imgui_sdl.h>
#include <imgui_internal.h>

#include <SDL.h>

#include "GameObject.h"
#include "sceneHierarchy.h"
#include "Transform.h"

#include "Component.hpp"
#include "MasterComponent.h"
#include "I_ComponentGUI.h"
#include "BasicMovementModuleGUI.h"
#include "SpriteRendererGUI.h"
#include "RectangleColliderGUI.h"
#include "AnonComponentGUI.h"
#include "BasicFollowAiGUI.h"
#include "colliderBounds.h"

#include "Profiler.h"
#include "Logger.h"

EditorDetailsPannel::EditorDetailsPannel(SDL_Window* gameWindow, SDL_Renderer* gameRenderer, sceneHierarchy** activeScene)
{
	currentGameObjectDetials.lock() = nullptr;
	_gameWindow = gameWindow;
	_gameRenderer = gameRenderer;
	_activeScene = activeScene;
}

EditorDetailsPannel::~EditorDetailsPannel()
{
	_gameWindow = nullptr;
	_gameRenderer = nullptr;
}

void EditorDetailsPannel::Update()
{
	//allows dounle clicking on an item in scene to view its items
	if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		auto mouseLocation = ImGui::GetMousePos();

		for (auto cgo : (*_activeScene)->getHierarchy())
		{
			if (cgo->isInBounds(vec2(mouseLocation.x, mouseLocation.y)))
			{
				this->changeGameObject(cgo);
			}
		}
	}

	//if no gameObject is selected the window isnt created
	if (!currentGameObjectDetials.lock())
		return;

	profiler->startProfile("EDetailsUpdate");

	SDL_SetRenderDrawColor(_gameRenderer, 0, 0, 255, 255);

	currentGameObjectDetials.lock()->updateRectBounds();
	SDL_Rect r;
	//r.x = currentGameObjectDetials.lock()->getTransform().Position().x;
	auto rectBounds = currentGameObjectDetials.lock()->getRectBounds();
	r.x = (rectBounds.origin + rectBounds.centre).x;
	//r.y = currentGameObjectDetials.lock()->getTransform().Position().y;
	r.y = (rectBounds.origin + rectBounds.centre).y;
	r.w = 50;
	r.h = 5;

	SDL_RenderFillRect(_gameRenderer, &r);
	SDL_SetRenderDrawColor(_gameRenderer, 255, 0, 0, 255);

	r.w = 5;
	r.h = -50;
	SDL_RenderFillRect(_gameRenderer, &r);

	ImGui::Begin("Details pannel");

	if (ImGui::Button("Close Details pannel"))
	{
		//if the close button is clicked then it will set gameObject to nullptr so that the pannel isnt show on the next loop
		currentGameObjectDetials.reset();
		ImGui::End();
		profiler->endProfile("EDetailsUpdate");
		return;
	}

	ImGui::Text("Game object name: ");
	ImGui::SameLine();
	//setting up for input
	static char inputbuffer[64] = "";
	static bool wasInputtedNameTaken = false;
	static bool wasInputEmpty = false;
	char namebuffer[32];
	sprintf_s(namebuffer, currentGameObjectDetials.lock()->getGameObjectName().c_str());
	ImGui::Button(namebuffer);
	//opens submenu to take user input
	if (ImGui::BeginPopupContextItem("GameObjectNameRename", ImGuiPopupFlags_MouseButtonLeft))
	{
		ImGui::Text("Edit name:");
		ImGui::InputText("", inputbuffer, 64);

		if (wasInputtedNameTaken)
			ImGui::Text("Name must be unique");

		if (wasInputEmpty)
			ImGui::Text("Name cannot be empty");

		//when the enter button is clicked then it will check if the name already exists if it doesnt it will change the name of the gameObject and update the gameObjectName map
		//if it already exists then it will display a message saying that names must be unique
		if (ImGui::Button("Enter"))
		{
			wasInputtedNameTaken = false;
			wasInputEmpty = false;

			std::string inputtedName = inputbuffer;
			if (inputtedName == "")
				wasInputEmpty = true;
			else if ((*_activeScene)->isGameObjectNameTaken(inputtedName))
				wasInputtedNameTaken = true;
			else
			{
				std::string oldName = currentGameObjectDetials.lock()->getGameObjectName();
				currentGameObjectDetials.lock()->setNameOfGameObject(inputtedName);
				(*_activeScene)->updateNameIDMap(currentGameObjectDetials.lock(), oldName);
			}
		}

		if (ImGui::Button("Close"))
		{
			wasInputtedNameTaken = false;
			inputbuffer[0] = '\0';
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}


	//draw details about the gameObject

	std::string currentStatus;
	if (currentGameObjectDetials.lock()->getActiveStatus())
		currentStatus = "Active";
	else
		currentStatus = "Inactive";

	bool activePTR = currentGameObjectDetials.lock()->getActiveStatus();
	ImGui::Checkbox("##currentGameObjectActive", &activePTR);
	currentGameObjectDetials.lock()->setActiveStatus((activePTR));
	ImGui::SameLine();
	ImGui::Text("Active status: %s", currentStatus.c_str());

	

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Transform - position");

		//starts the line
		ImGui::Text("X: ");
		ImGui::SameLine();

		//create a buffer that the input will go into and be read from
		static char buffer[32] = "";
		//adds the current x val into the buffer
		sprintf_s(buffer, std::to_string(currentGameObjectDetials.lock()->getTransform().Position().x).c_str());
		//get the input from the user
		ImGui::InputText("##traposx", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
		//sets a default value if the input is empty
		int valAsInt = 0;
		//checks that the array isnt empty - if it isnt get the inputed char as a int
		if (buffer[0] != '\0')
			valAsInt = std::stoi(buffer);
		//updates with the new value 
		currentGameObjectDetials.lock()->setPositionTransform(vec2(valAsInt, currentGameObjectDetials.lock()->getTransform().Position().y));


		buffer[0] = '\0';
		//updates the buffer to be the y val
		sprintf_s(buffer, std::to_string(currentGameObjectDetials.lock()->getTransform().Position().y).c_str());
		ImGui::Text("Y: ");
		ImGui::SameLine();
		//takes the input for the y val
		ImGui::InputText("##traposy", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
		//resets to default value
		valAsInt = 0;
		//checks there is a value and updates the int with the inputed value
		if (buffer[0] != '\0')
			valAsInt = std::stoi(buffer);
		//updates the gameObject value
		currentGameObjectDetials.lock()->setPositionTransform(vec2(currentGameObjectDetials.lock()->getTransform().Position().x, valAsInt));

		//cant change scale - it doesnt do anything anyway 
		ImGui::Text("Transform - scale");
		ImGui::Text("X: %d", currentGameObjectDetials.lock()->getTransform().Scale().x);
		ImGui::Text("Y: %d", currentGameObjectDetials.lock()->getTransform().Scale().y);

		ImGui::Text("Local Transform - position");

		ImGui::Text("X: ");
		ImGui::SameLine();

		buffer[0] = '\0';
		sprintf_s(buffer, std::to_string(currentGameObjectDetials.lock()->getLocalTransform().Position().x).c_str());
		ImGui::InputText("##loctraposx", buffer, 32, ImGuiInputTextFlags_CharsDecimal);

		valAsInt = 0;
		if (buffer[0] != '\0')
			valAsInt = std::stoi(buffer);

		currentGameObjectDetials.lock()->getLocalTransform().Position().x = valAsInt;


		ImGui::Text("Y: ");
		ImGui::SameLine();

		buffer[0] = '\0';
		sprintf_s(buffer, std::to_string(currentGameObjectDetials.lock()->getLocalTransform().Position().y).c_str());
		ImGui::InputText("##loctraposy", buffer, 32, ImGuiInputTextFlags_CharsDecimal);

		valAsInt = 0;
		if (buffer[0] != '\0')
			valAsInt = std::stoi(buffer);

		currentGameObjectDetials.lock()->getLocalTransform().Position().y = valAsInt;

		ImGui::Text("Local Transform - scale");
		ImGui::Text("X: %d", currentGameObjectDetials.lock()->getTransform().Scale().x);
		ImGui::Text("Y: %d", currentGameObjectDetials.lock()->getTransform().Scale().y);
	}

	if (ImGui::CollapsingHeader("Components"))
	{
		int compIndex = 0;
		for (I_ComponentGUI* compGUI : currentGameObjectComponets)
		{
			std::string treenodeTag = compGUI->getComponetName() + "##" + std::to_string(compIndex);
			if (ImGui::TreeNode(treenodeTag.c_str()))
			{
				compGUI->Draw();

				if (ImGui::Button("Delete Component"))
				{
					currentGameObjectDetials.lock()->deleteComponent(compGUI->getPtrToBaseComp());
					changeGameObject(currentGameObjectDetials.lock());
					ImGui::TreePop();
					break;	
				}

				ImGui::TreePop();
			}		

			compIndex++;
		}

		ImGui::Button("Add component");
		
		if (ImGui::BeginPopupContextItem("##acomp", ImGuiMouseButton_Left))
		{
			if(ImGui::Button("Add basic movement module"))
			{
				(*_activeScene)->AddBasicMovementModule(currentGameObjectDetials.lock());
				currentGameObjectDetials.lock()->initialize();
				changeGameObject(currentGameObjectDetials.lock());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Add Rectangle Collider"))
			{
				(*_activeScene)->AddRectangleCollider(currentGameObjectDetials.lock());
				currentGameObjectDetials.lock()->initialize();
				changeGameObject(currentGameObjectDetials.lock());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Add Test Game Comp"))
			{
				(*_activeScene)->AddTestGameComp(currentGameObjectDetials.lock());
				currentGameObjectDetials.lock()->initialize();
				changeGameObject(currentGameObjectDetials.lock());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Add Basic Follow Ai Comp"))
			{
				(*_activeScene)->AddBasicFollowAi(currentGameObjectDetials.lock());
				currentGameObjectDetials.lock()->initialize();
				changeGameObject(currentGameObjectDetials.lock());
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();
	profiler->endProfile("EDetailsUpdate");
}

void EditorDetailsPannel::changeGameObject(std::shared_ptr<GameObject> newGameObject)
{
	for (I_ComponentGUI* Icomp : currentGameObjectComponets)
	{
		delete Icomp;
		Icomp = nullptr;
	}
	currentGameObjectComponets.clear();

	currentGameObjectDetials = newGameObject;

	for (Component* comp : currentGameObjectDetials.lock()->getTotalComponentsVector())
	{
		SpriteRendererGUI* tempSR;
		BasicMovementModuleGUI* tempBMM;
		RectangleColliderGUI* tempRC;
		BasicFollowAIGUI* tempBFA;
		AnonComponentGUI* tempAnon;

		switch (comp->getComponentClass())
		{
		case ComponetClass::CC_spriteRenderer:
			tempSR = new SpriteRendererGUI((SpriteRenderer*)comp);
			currentGameObjectComponets.push_back(tempSR);
			break;
		case ComponetClass::CC_basicMovementModule:
			tempBMM = new BasicMovementModuleGUI((BasicMovementModule*)comp);
			currentGameObjectComponets.push_back(tempBMM);
			break;
		case ComponetClass::CC_rectangleCollider:
			tempRC = new RectangleColliderGUI((RectangleCollider*)comp);
			currentGameObjectComponets.push_back(tempRC);
			break;
		case ComponetClass::CC_BasicFollowAI:
			tempBFA = new BasicFollowAIGUI((BasicFollowAI*)comp);
			currentGameObjectComponets.push_back(tempBFA);
			break;
		default:
			tempAnon = new AnonComponentGUI(comp);
			currentGameObjectComponets.push_back(tempAnon);
			break;
		}
	}
}

void EditorDetailsPannel::removeGameObject()
{
	currentGameObjectDetials.lock() = nullptr;

	currentGameObjectComponets.clear();
}

void EditorDetailsPannel::popUpWindow()
{
	//currently not used 

	if (ImGui::BeginPopupContextItem("DetailsPopUpWindow"))
	{

		ImGui::EndPopup();
	}

	ImGui::OpenPopupOnItemClick("DetailsPopUpWindow", ImGuiPopupFlags_MouseButtonRight);
}
