#include "AnonComponentGUI.h"
#include "Component.hpp"
#include <imgui.h>

const std::string& AnonComponentGUI::getComponetName()
{
	return compPtr->getComponentName();
}

void AnonComponentGUI::Draw()
{
	std::string currentStatus;
	if (compPtr->getActiveStatus())
		currentStatus = "Active";
	else
		currentStatus = "Inactive";

	bool activePTR = compPtr->getActiveStatus();
	ImGui::Checkbox("##BasicMovementModuleActive", &activePTR);
	compPtr->setActiveStatus((activePTR));
	ImGui::SameLine();
	ImGui::Text("Active status: %s", currentStatus.c_str());
}

ComponetClass AnonComponentGUI::getComponetType()
{
	return compPtr->getComponentClass();
}

Component* AnonComponentGUI::getPtrToBaseComp()
{
	return compPtr;
}
