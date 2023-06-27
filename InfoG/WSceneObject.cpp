#include "WSceneObject.h"

void WSceneObject::renderChildNodes(Node child)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    if (std::find(RObjectManager::selectedIds.begin(), RObjectManager::selectedIds.end(), child.getID()) != RObjectManager::selectedIds.end()) {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool isOpened = ImGui::TreeNodeEx((RObjectManager::getRObject(child.getRObjects().at(0)).getName()).c_str(), node_flags);
    if (ImGui::IsItemClicked()) {
        if (!ImGui::GetIO().KeyCtrl) RObjectManager::selectedIds.clear();
        RObjectManager::selectedIds.push_back(child.getID());
    }


    if (isOpened) {
        for (int i = 0; i < child.getRObjects().size(); i++)
        {
            if (ImGui::Selectable(RObjectManager::getRObject(child.getRObjects().at(i)).getName().c_str(),
                std::find(RObjectManager::selectedIds.begin(), RObjectManager::selectedIds.end(), child.getRObjects().at(i)) != RObjectManager::selectedIds.end()))
            {
                if (!ImGui::GetIO().KeyCtrl) RObjectManager::selectedIds.clear();
                RObjectManager::selectedIds.push_back(child.getRObjects().at(i));
            }
        }

        for (Node node : child.getChildNodes()) {
            renderChildNodes(node);
        }

        ImGui::TreePop();
    }
}

void WSceneObject::renderLights(Light light) {

    std::string lightNames[3] {"SpotLight", "DirectionalLight", "PointLight"};

    if (ImGui::Selectable((lightNames[light.lightType] + " #" + std::to_string(light.getID())).c_str(),
        std::find(RObjectManager::selectedIds.begin(), RObjectManager::selectedIds.end(), light.getID()) != RObjectManager::selectedIds.end()))
    {
        if (!ImGui::GetIO().KeyCtrl) RObjectManager::selectedIds.clear();
        RObjectManager::selectedIds.push_back(light.getID());
    }
}

void WSceneObject::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Scene Objects", &p_open))
    {
        ImGui::End();
        return;
    }

    //bool DNDNodes = true;

    // tree list ? and we gotta get the camera in too

    {
        ImGui::TextDisabled("(?) ");
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted("Hold CTRL and click to select multiple items.");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
            SetCursor(LoadCursor(NULL, IDC_HELP));
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete")) {
            for (unsigned int cID : RObjectManager::selectedIds) {
                RObjectManager::removeCamera(cID);
                RObjectManager::removeLight(cID);
                RObjectManager::removeRObject(cID);
                RObjectManager::removeNode(cID);
            }
        }

        for (const auto& [key, node] : RObjectManager::getNodes()) {
            renderChildNodes(node);
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        for (const auto& [key, light] : RObjectManager::getLights()) {
            renderLights(*light);
        }

        //TODO drag and drop the nodes into one another
    }

    if (notified) {
        if (ImGui::IsWindowHovered()) RObjectManager::loadRObject(DNDFileManager::getPaths().front());
        notified = false;
    }
    ImGui::End();
}


