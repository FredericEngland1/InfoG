#include "WProperties.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

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

        // TODO add light to the frey

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

            // TODO make a way to change the current shader

            std::string VSPath = itO->second.getVSPath();
            std::string FSPath = itO->second.getFSPath();

            std::string GSPath = "";

            // TODO disable geo shader ?

            bool geo = !itO->second.getGSPath().empty();
            ImGui::Checkbox("Geometry Shader", &geo);

            if (geo) {
                GSPath = "Geometry.shader";
            }

            itO->second.setShader(VSPath, FSPath, GSPath);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            std::string matName = itO->second.getMaterial().name.c_str();
            matName.resize(128);

            ImGui::Text("Material : ");

            ImGui::SameLine();
            ImGui::InputText("##RO-MatN", &matName[0], matName.size());
            itO->second.getMaterialRef().name = matName;

            ImGui::Spacing();
            ImGui::Spacing();

            float ambientData[3]{ itO->second.getMaterial().ambient.x,itO->second.getMaterial().ambient.y,itO->second.getMaterial().ambient.z };

            ImGui::Text("Ambient R - G - B");
            ImGui::DragFloat3("##RO-MatA-rgb", ambientData, 0.5);
            itO->second.getMaterialRef().ambient = { ambientData[0],ambientData[1],ambientData[2] };

            ImGui::Spacing();
            ImGui::Spacing();

            float diffuseData[3]{ itO->second.getMaterial().diffuse.x,itO->second.getMaterial().diffuse.y,itO->second.getMaterial().diffuse.z };

            ImGui::Text("Diffuse R - G - B");
            ImGui::DragFloat3("##RO-MatD-rgb", diffuseData, 0.5);
            itO->second.getMaterialRef().diffuse = { diffuseData[0],diffuseData[1],diffuseData[2] };

            ImGui::Spacing();
            ImGui::Spacing();

            float specularData[3]{ itO->second.getMaterial().specular.x,itO->second.getMaterial().specular.y,itO->second.getMaterial().specular.z };

            ImGui::Text("Specular R - G - B");
            ImGui::DragFloat3("##RO-MatS-rgb", specularData, 0.5);
            itO->second.getMaterialRef().specular = { specularData[0],specularData[1],specularData[2] };

            ImGui::Spacing();
            ImGui::Spacing();

            float shininessData = itO->second.getMaterial().shininess;

            ImGui::Text("Shininess : ");
            ImGui::SameLine();
            ImGui::DragFloat("##RO-MatShiny-rgb", &shininessData, 0.5);
            itO->second.getMaterialRef().shininess = shininessData;

            ImGui::Spacing();
            ImGui::Spacing();
            
            if (ImGui::Button("Randomize DiffuseMap")) {
                itO->second.getMaterialRef().diffuseMap.randomImage();
            }

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();


            // TODO Texture diffuseMap;

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

            DirectionalLight* drl;
            PointLight* pl;
            SpotLight* sl;

            float postitionData[3];

            float ambient[3];
            float diffuse[3];
            float specular[3];

            float direction[3];

            float constant;
            float linear;
            float quadratic;

            float cutOff;
            float outerCutOff;

            switch (itL->second->lightType) {
            case LightType::directional:

                drl = (DirectionalLight*)(itL->second);

                direction[0] = drl->direction.x;
                direction[1] = drl->direction.y;
                direction[2] = drl->direction.z;

                ImGui::Text("Direction X - Y - Z");
                ImGui::DragFloat3("##RL-direction", direction, 0.5);
                drl->direction = { direction[0],direction[1],direction[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                diffuse[0] = drl->diffuse.x;
                diffuse[1] = drl->diffuse.y;
                diffuse[2] = drl->diffuse.z;

                ImGui::Text("Diffuse R - G - B");
                ImGui::DragFloat3("##RL-diffuse", diffuse, 0.5);
                drl->diffuse = { diffuse[0],diffuse[1],diffuse[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                ambient[0] = drl->ambient.x;
                ambient[1] = drl->ambient.y;
                ambient[2] = drl->ambient.z;

                ImGui::Text("Ambiant R - G - B");
                ImGui::DragFloat3("##RL-ambiant", ambient, 0.5);
                drl->ambient = { ambient[0],ambient[1],ambient[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                specular[0] = drl->specular.x;
                specular[1] = drl->specular.y;
                specular[2] = drl->specular.z;

                ImGui::Text("Specular R - G - B");
                ImGui::DragFloat3("##RL-specular", specular, 0.5);
                drl->specular = { specular[0],specular[1],specular[2] };

                break;
            case LightType::point:

                pl = (PointLight*)(itL->second);

                postitionData[0] = pl->position.x;
                postitionData[1] = pl->position.y;
                postitionData[2] = pl->position.z;

                ImGui::Text("Position X - Y - Z");
                ImGui::DragFloat3("##RL-P-xyz", postitionData, 0.5);
                pl->position = { postitionData[0],postitionData[1],postitionData[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                diffuse[0] = pl->diffuse.x;
                diffuse[1] = pl->diffuse.y;
                diffuse[2] = pl->diffuse.z;

                ImGui::Text("Diffuse R - G - B");
                ImGui::DragFloat3("##RL-diffuse", diffuse, 0.5);
                pl->diffuse = { diffuse[0],diffuse[1],diffuse[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                ambient[0] = pl->ambient.x;
                ambient[1] = pl->ambient.y;
                ambient[2] = pl->ambient.z;

                ImGui::Text("Ambiant R - G - B");
                ImGui::DragFloat3("##RL-ambiant", ambient, 0.5);
                pl->ambient = { ambient[0],ambient[1],ambient[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                specular[0] = pl->specular.x;
                specular[1] = pl->specular.y;
                specular[2] = pl->specular.z;

                ImGui::Text("Specular R - G - B");
                ImGui::DragFloat3("##RL-specular", specular, 0.5);
                pl->specular = { specular[0],specular[1],specular[2] };

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Falloff");

                constant = pl->constant;

                ImGui::Text("Constant : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-constant", &constant, 0.5);
                pl->constant = constant;

                linear = pl->linear;

                ImGui::Text("Linear : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-linear", &linear, 0.5);
                pl->linear = linear;

                quadratic = pl->quadratic;

                ImGui::Text("Quadratic : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-quadratic", &quadratic, 0.5);
                pl->quadratic = quadratic;

                break;

            case LightType::spot:

                sl = (SpotLight*)(itL->second);

                postitionData[0] = sl->position.x;
                postitionData[1] = sl->position.y;
                postitionData[2] = sl->position.z;

                ImGui::Text("Position X - Y - Z");
                ImGui::DragFloat3("##RL-P-xyz", postitionData, 0.5);
                sl->position = { postitionData[0],postitionData[1],postitionData[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                direction[0] = sl->direction.x;
                direction[1] = sl->direction.y;
                direction[2] = sl->direction.z;

                ImGui::Text("Direction X - Y - Z");
                ImGui::DragFloat3("##RL-direction", direction, 0.5);
                sl->direction = { direction[0],direction[1],direction[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                diffuse[0] = sl->diffuse.x;
                diffuse[1] = sl->diffuse.y;
                diffuse[2] = sl->diffuse.z;

                ImGui::Text("Diffuse R - G - B");
                ImGui::DragFloat3("##RL-diffuse", diffuse, 0.5);
                sl->diffuse = { diffuse[0],diffuse[1],diffuse[2] };

                ImGui::Spacing();
                ImGui::Spacing();



                ambient[0] = sl->ambient.x;
                ambient[1] = sl->ambient.y;
                ambient[2] = sl->ambient.z;

                ImGui::Text("Ambiant R - G - B");
                ImGui::DragFloat3("##RL-ambiant", ambient, 0.5);
                sl->ambient = { ambient[0],ambient[1],ambient[2] };

                ImGui::Spacing();
                ImGui::Spacing();

                specular[0] = sl->specular.x;
                specular[1] = sl->specular.y;
                specular[2] = sl->specular.z;

                ImGui::Text("Specular R - G - B");
                ImGui::DragFloat3("##RL-specular", specular, 0.5);
                sl->specular = { specular[0],specular[1],specular[2] };

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Falloff");

                constant = sl->constant;

                ImGui::Text("Constant : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-constant", &constant, 0.5);
                sl->constant = constant;

                linear = sl->linear;

                ImGui::Text("Linear : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-linear", &linear, 0.5);
                sl->linear = linear;

                quadratic = sl->quadratic;

                ImGui::Text("Quadratic : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-quadratic", &quadratic, 0.5);
                sl->quadratic = quadratic;

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::Text("Cutoff");

                cutOff = sl->cutOff;

                ImGui::Text("CutOff : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-cutOff", &cutOff, 0.5);
                sl->cutOff = cutOff;

                outerCutOff = sl->outerCutOff;

                ImGui::Text("OuterCutOff : ");
                ImGui::SameLine();
                ImGui::DragFloat("##RL-outerCutOff", &outerCutOff, 0.5);
                sl->outerCutOff = outerCutOff;



                break;
            }

            ImGui::End();
            return;
        }
    }

    ImGui::End();
}
