#include "BasicMovementModuleGUI.h"
#include <imgui.h>
#include "BasicMovementModule.h"
#include "Component.hpp"



const std::string& BasicMovementModuleGUI::getComponetName()
{
	return BMM->getComponentName();
}

void BasicMovementModuleGUI::Draw()
{
	//ImGui::Text("Basic movement module");

	std::string currentStatus;
	if (BMM->getActiveStatus())
		currentStatus = "Active";
	else
		currentStatus = "Inactive";

	bool activePTR = BMM->getActiveStatus();
	ImGui::Checkbox("##BasicMovementModuleActive", &activePTR);
	BMM->setActiveStatus((activePTR));
	ImGui::SameLine();
	ImGui::Text("Active status: %s", currentStatus.c_str());

	ImGui::Text("...Movement speed: ");
	ImGui::SameLine();
	static char inputbuffer[64] = "";
	char speedValueBuffer[32];
	//the ##BMM is used to make sure all the ids are different but if another bmm with the same speed exists it will have the same id
	sprintf_s(speedValueBuffer,"%d##BMM", BMM->getSpeedToMove());
	//ImGui::Text();

	//ImGui::InputText("##inputSpeedForBMM", inputbuffer, 64, ImGuiInputTextFlags_CharsDecimal);

	ImGui::Button(speedValueBuffer);
	if (ImGui::BeginPopupContextItem("basicMovementModuleChangeValue", ImGuiPopupFlags_MouseButtonLeft))
	{
		ImGui::Text("Edit speed:");
		ImGui::InputText("", inputbuffer, 64, ImGuiInputTextFlags_CharsDecimal);
		if (ImGui::Button("Enter"))
		{
			int inputtedValue = std::stoi(inputbuffer);
			BMM->setSpeedToMove(inputtedValue);
		}

		if (ImGui::Button("Close"))
		{
			inputbuffer[0] = '\0';
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

ComponetClass BasicMovementModuleGUI::getComponetType()
{
	return BMM->getComponentClass();
}

Component* BasicMovementModuleGUI::getPtrToBaseComp()
{
	return BMM;
}
