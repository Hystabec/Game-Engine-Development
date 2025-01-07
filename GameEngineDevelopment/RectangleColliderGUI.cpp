#include "RectangleColliderGUI.h"
#include <imgui.h>
#include "RectangleCollider.h"

const std::string& RectangleColliderGUI::getComponetName()
{
    return rectCol->getComponentName();
}

void RectangleColliderGUI::Draw()
{
    bool activePTR = rectCol->getActiveStatus();
    ImGui::Checkbox("##RectColActive", &activePTR);
    rectCol->setActiveStatus(activePTR);
    ImGui::SameLine();
    ImGui::Text("Active status: %s", rectCol->getActiveStatus() ? "Active" : "Inactive");

    static char buffer[32] = "";
    sprintf_s(buffer, rectCol->Tag.c_str());
    
    ImGui::Checkbox("Trigger", &rectCol->Trigger);
    
    ImGui::Text("Trigger Tag: ");
    ImGui::SameLine();
    if (ImGui::InputText("##tagInput", buffer, 32, ImGuiInputTextFlags_EnterReturnsTrue))
        rectCol->Tag = buffer;
}

ComponetClass RectangleColliderGUI::getComponetType()
{
    return rectCol->getComponentClass();
}

Component* RectangleColliderGUI::getPtrToBaseComp()
{
    return rectCol;
}
