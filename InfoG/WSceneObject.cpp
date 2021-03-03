#include "WSceneObject.h"

void WSceneObject::render()
{
    /*ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    bool DNDNodes = true;*/

    // tree list ? and we gotta get the camera in too

    ImGui::Begin("Scene Objects");

    for (auto& [key, value] : RObjectManager::getRObjects())
    {
        ImGui::Text(value.getName().c_str());
    }

    if (notified) {
        if (ImGui::IsWindowHovered()) RObjectManager::loadRObject(DNDFileManager::getPaths().front());
        notified = false;
    }

    ImGui::End();
}


