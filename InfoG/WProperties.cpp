#include "WProperties.h"

void WProperties::render()
{
    if (!p_open) return;

    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Properties", &p_open))
    {
        ImGui::End();
        return;
    }

    bool sameSelection = (previousSelection == RObjectManager::selectedIds);
    previousSelection = RObjectManager::selectedIds;

    if (!sameSelection) {
        position = { 0,0,0 };
        rotation = { 0,0,0 };
        scale = { 0,0,0 };
        degreeRotation = 0;
    }

    if (RObjectManager::selectedIds.size() == 0) {
        ImGui::TextDisabled("No item currently selected");
        if (ImGui::IsItemHovered()) SetCursor(LoadCursor(NULL, IDC_NO));
    }
    else if (RObjectManager::selectedIds.size() > 1) {

        ImGui::TextDisabled("The data shown represents the difference");
        ImGui::TextDisabled("of all value since the selection was made.");
        if (ImGui::IsItemHovered()) SetCursor(LoadCursor(NULL, IDC_NO));

        ImGui::Spacing();

        float postitionData[3]{ position.x,position.y,position.z };

        ImGui::Text("Position X - Y - Z");
        ImGui::DragFloat3("##RO-P-xyz", postitionData, 0.5);

        float rotationData[4]{ rotation.x,rotation.y,rotation.z, degreeRotation };

        ImGui::Text("Rotation X - Y - Z - Degree");
        ImGui::DragFloat4("##RO-R-xyz", rotationData, 0.1);

        float scaleData[3]{ scale.x,scale.y,scale.z };

        ImGui::Text("Scale X - Y - Z");
        ImGui::DragFloat3("##RO-S-xyz", scaleData, 0.1);

        for (unsigned int cID : RObjectManager::selectedIds) { // only checks for RObjects
            auto itO = RObjectManager::getRObjects().find(cID);

            if (itO != RObjectManager::getRObjects().end()) {
                itO->second.setPosition({ itO->second.getPosition().x + (postitionData[0] - position.x), itO->second.getPosition().y + (postitionData[1] - position.y), itO->second.getPosition().z + (postitionData[2] - position.z) });
                itO->second.setRotation({ itO->second.getRotation().x + (rotationData[0] - rotation.x), itO->second.getRotation().y + (rotationData[1] - rotation.y), itO->second.getRotation().z + (rotationData[2] - rotation.z)});
                itO->second.setDegreeRotation({ itO->second.getDegreeRotation() + (rotationData[3] - degreeRotation)});
                itO->second.setScale({ itO->second.getScale().x + (scaleData[0] - scale.x), itO->second.getScale().y + (scaleData[1] - scale.y), itO->second.getScale().z + (scaleData[2] - scale.z)});
            }
        }

        position = { postitionData[0], postitionData[1], postitionData[2] };
        rotation = { rotationData[0], rotationData[1],rotationData[2] };
        scale = { scaleData[0], scaleData[1], scaleData[2] };
        degreeRotation = rotationData[3];
    }
    else {
        unsigned int ID = RObjectManager::selectedIds.at(0);
        auto itO = RObjectManager::getRObjects().find(ID);

        if (itO != RObjectManager::getRObjects().end()) {

            std::string name = itO->second.getName().c_str();
            name.resize(128);

            ImGui::Text("Name : ");
            ImGui::SameLine();
            ImGui::InputText("##RO-N", &name[0], name.size());
            itO->second.setName(name);
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            float postitionData[3]{ itO->second.getPosition().x,itO->second.getPosition().y,itO->second.getPosition().z };

            ImGui::Text("Position X - Y - Z");
            ImGui::DragFloat3("##RO-P-xyz", postitionData, 0.5);
            itO->second.setPosition({ postitionData[0],postitionData[1],postitionData[2] });

            float rotationData[4]{ itO->second.getRotation().x,itO->second.getRotation().y,itO->second.getRotation().z, itO->second.getDegreeRotation() };

            ImGui::Text("Rotation X - Y - Z - Degree");
            ImGui::DragFloat4("##RO-R-xyz", rotationData, 0.1);
            itO->second.setRotation({ rotationData[0],rotationData[1],rotationData[2] });
            itO->second.setDegreeRotation({ rotationData[3] });

            float scaleData[3]{ itO->second.getScale().x,itO->second.getScale().y,itO->second.getScale().z };

            ImGui::Text("Scale X - Y - Z");
            ImGui::DragFloat3("##RO-S-xyz", scaleData, 0.1);
            itO->second.setScale({ scaleData[0],scaleData[1],scaleData[2] });

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Focus")) {
                for (auto &[key, camera] : RObjectManager::getCameras()) {
                    camera->setLookAt(itO->second.getPosition());
                }
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            std::string VSPath = itO->second.getVSPath();
            std::string FSPath = itO->second.getFSPath();

            std::string GSPath = "";

            bool geo = !itO->second.getGSPath().empty();
            ImGui::Checkbox("Geometry Shader", &geo);

            if (geo) {
                GSPath = "Geometry.shader";
            }

            itO->second.setShader(VSPath, FSPath, GSPath);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

            openHisto = ImGui::TreeNodeEx("Histogram", node_flags);

            if (openHisto) {
                ImGui::Text("Histogram of the diffuse map");
                ImGui::Text("(Order) R - G - B");

                std::vector<cv::Mat> rgb;
                cv::split(itO->second.getMaterial().diffuseMap.getImage().getImage(), rgb);

                float range[] = { 0, 256 };
                const float* histRange = { range };

                int histogramSize = 256;

                cv::Mat r;
                cv::Mat g;
                cv::Mat b;
                calcHist(&rgb[0], 1, 0, cv::Mat::Mat(), r, 1, &histogramSize, &histRange, true, false);
                calcHist(&rgb[1], 1, 0, cv::Mat::Mat(), g, 1, &histogramSize, &histRange, true, false);
                calcHist(&rgb[2], 1, 0, cv::Mat::Mat(), b, 1, &histogramSize, &histRange, true, false);

                float rd[256]{ 0 };
                float gd[256]{ 0 };
                float bd[256]{ 0 };

                for (int i = 0; i < histogramSize; ++i) {
                    rd[i] = (*r.ptr<float>(i));
                    gd[i] = (*g.ptr<float>(i));
                    bd[i] = (*b.ptr<float>(i));
                }

                float maxR = *std::max_element(rd, rd + 256);
                float maxG = *std::max_element(gd, gd + 256);
                float maxB = *std::max_element(bd, bd + 256);

                float maxRG = std::max(maxR, maxG);
                float maxRGB = std::max(maxRG, maxB);

                ImGui::PlotHistogram("##RH-WP", rd, IM_ARRAYSIZE(rd), 0, NULL, 0.0f, maxRGB, ImVec2(0, 80));
                ImGui::PlotHistogram("##GH-WP", gd, IM_ARRAYSIZE(gd), 0, NULL, 0.0f, maxRGB, ImVec2(0, 80));
                ImGui::PlotHistogram("##BH-WP", bd, IM_ARRAYSIZE(bd), 0, NULL, 0.0f, maxRGB, ImVec2(0, 80));

                ImGui::TreePop();
            }

            ImGui::End();
            return;
        }

        auto itN = RObjectManager::getNodes().find(ID);

        if (itN != RObjectManager::getNodes().end()) {



            ImGui::End();
            return;
        }

        auto itC = RObjectManager::getCameras().find(ID);

        if (itC != RObjectManager::getCameras().end()) {

            return;
        }

        auto itL = RObjectManager::getLights().find(ID);

        if (itL != RObjectManager::getLights().end()) {

            ImGui::End();
            return;
        }
    }

    ImGui::End();
}
