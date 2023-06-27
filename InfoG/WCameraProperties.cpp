#include "WCameraProperties.h"

WCameraProperties::WCameraProperties(Camera& camera) : camera(camera)
{
}

void WCameraProperties::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Camera Properties", &p_open))
    {
        ImGui::End();
        return;
    }


}
