#include "WCamera.h"

WCamera::WCamera(Camera& camera, std::string name, glm::vec4 clearColor) : camera(camera), name(name), clearColor(clearColor)
{
    renderer = Renderer(camera);
}

void WCamera::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin(name.c_str(), &p_open))
    {
        ImGui::End();
        return;
    }
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();

        unsigned int tID = renderer.render(ImGui::GetWindowWidth(), ImGui::GetWindowHeight(), clearColor);
        
        ImGui::GetWindowDrawList()->AddImage(
            (void*)tID,
            ImVec2(ImGui::GetCursorScreenPos()),
            ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
                ImGui::GetCursorScreenPos().y + ImGui::GetWindowHeight()),
            ImVec2(0, 1), 
            ImVec2(1, 0)
        );
    }
    ImGui::End();
}