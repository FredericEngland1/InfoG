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
        if (ImGui::IsWindowHovered()) {
            if (ImGui::IsMouseDown(1)) {

                float distance = glm::distance(camera.getLookAt(), camera.getPosition());

                float circ = distance * 2 * (acos(0.0) * 2);

                auto xMat = glm::rotate((float)(((ImGui::GetMouseDragDelta(1).x / circ) * 360) * (acos(0.0) * 2)) / 180, glm::vec3(0,1,0));

                camera.setPosition(xMat * glm::rotate((float)(((ImGui::GetMouseDragDelta(1).y / circ) * 360) * (acos(0.0) * 2)) / 180, glm::vec3(1,0,0)) * glm::vec4(camera.getPosition(), 0));

                ImGui::ResetMouseDragDelta(1);
            }
        }

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