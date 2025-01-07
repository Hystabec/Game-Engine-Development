#include "BasicFollowAiGUI.h"
#include <imgui.h>
#include "BasicFollowAI.h"
#include "Component.hpp"

const std::string& BasicFollowAIGUI::getComponetName()
{
	return BFA->getComponentName();
}

void BasicFollowAIGUI::Draw()
{
	//setting up the active checkbox and text
	std::string currentStatus;
	if (BFA->getActiveStatus())
		currentStatus = "Active";
	else
		currentStatus = "Inactive";

	bool activePTR = BFA->getActiveStatus();
	ImGui::Checkbox("##BasicMovementModuleActive", &activePTR);
	BFA->setActiveStatus((activePTR));
	ImGui::SameLine();
	ImGui::Text("Active status: %s", currentStatus.c_str());

	ImGui::Text("Patrol point one");
	ImGui::Text("X: ");
	ImGui::SameLine();

	static char buffer[32] = "";

	sprintf_s(buffer, std::to_string(BFA->getPositionOne().x).c_str());
	ImGui::InputText("##ppox", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
	int valAsInt = 0;
	if (buffer[0] != '\0')
		valAsInt = std::stoi(buffer);
	BFA->changePointOne(vec2(valAsInt, BFA->getPositionOne().y));

	ImGui::Text("Y: ");
	ImGui::SameLine();
	buffer[0] = '\0';
	sprintf_s(buffer, std::to_string(BFA->getPositionOne().y).c_str());
	ImGui::InputText("##ppoy", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
	valAsInt = 0;
	if (buffer[0] != '\0')
		valAsInt = std::stoi(buffer);
	BFA->changePointOne(vec2(BFA->getPositionOne().x, valAsInt));

	ImGui::Text("Patrol point two");
	ImGui::Text("X: ");
	ImGui::SameLine();
	buffer[0] = '\0';
	sprintf_s(buffer, std::to_string(BFA->getPositionTwo().x).c_str());
	ImGui::InputText("##pptx", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
	valAsInt = 0;
	if (buffer[0] != '\0')
		valAsInt = std::stoi(buffer);
	BFA->changePointTwo(vec2(valAsInt, BFA->getPositionTwo().y));

	ImGui::Text("Y: ");
	ImGui::SameLine();
	buffer[0] = '\0';
	sprintf_s(buffer, std::to_string(BFA->getPositionTwo().y).c_str());
	ImGui::InputText("##ppty", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
	valAsInt = 0;
	if (buffer[0] != '\0')
		valAsInt = std::stoi(buffer);
	BFA->changePointTwo(vec2(BFA->getPositionTwo().x, valAsInt));

	ImGui::Text("Movement speed : ");
	ImGui::SameLine();
	buffer[0] = '\0';
	sprintf_s(buffer, std::to_string(BFA->getMovementSpeed()).c_str());
	ImGui::InputText("##mms", buffer, 32, ImGuiInputTextFlags_CharsDecimal);
	valAsInt = 0;
	if (buffer[0] != '\0')
		valAsInt = std::stoi(buffer);
	BFA->changeMovementSpeed(valAsInt);
	buffer[0] = '\0';
}

ComponetClass BasicFollowAIGUI::getComponetType()
{
	return BFA->getComponentClass();
}

Component* BasicFollowAIGUI::getPtrToBaseComp()
{
	return BFA;
}
