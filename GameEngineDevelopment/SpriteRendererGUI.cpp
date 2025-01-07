#include "SpriteRendererGUI.h"
#include <imgui.h>
#include "SpriteRenderer.h"

const std::string& SpriteRendererGUI::getComponetName()
{
	return SpriteRend->getComponentName();
}

void SpriteRendererGUI::Draw()
{
	//ImGui::Text("Sprite renderer");
	std::string currentStatus;
	if (SpriteRend->getActiveStatus())
		currentStatus = "Active";
	else
		currentStatus = "Inactive";

	bool activePTR = SpriteRend->getActiveStatus();
	ImGui::Checkbox("##SpriteRendererActive", &activePTR);
	SpriteRend->setActiveStatus((activePTR));
	ImGui::SameLine();
	ImGui::Text("Active status: %s", currentStatus.c_str());

	ImGui::Text("File location:");
	ImGui::Text(SpriteRend->getFileLocation().c_str());
}

ComponetClass SpriteRendererGUI::getComponetType()
{
	return SpriteRend->getComponentClass();
}

Component* SpriteRendererGUI::getPtrToBaseComp()
{
	return SpriteRend;
}

