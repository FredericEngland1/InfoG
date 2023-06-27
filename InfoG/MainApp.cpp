#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <vector>
#include <cmath>

#include <glm/glm.hpp>

#include "Shader.h"
#include "DNDImgWidget.h"
#include "DNDMosaicWidget.h"
#include "DNDFileManager.h"

#include "WConsole.h"
#include "WSceneObject.h"
#include "WProperties.h"

#include "RObject.h"

#include "PerspectiveCamera.h"
#include "Camera.h"

#include <opencv2/opencv.hpp>
#include "Renderer.h"
#include "WCamera.h"
#include "OrthographicCamera.h"
#include "Cube.h"
#include "Pyramide.h"
#include "Fleche.h"
#include "PlusSign.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "WRaytracing.h"


GLFWwindow* window;
GLFWmonitor* monitor;

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;
const char *WIN_TITLE = "TestApp";

const char* glsl_version = "#version 460 core";

void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // We ignore the info dumps
    if (type == 0x8251) return;

    std::string error = ("GL_CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "* GL_ERROR *" : ""),
        type, severity, message);

    //MainWConsole::mainConsole.addLog(error, LogType::error);
}

int main()
{
    if (glfwInit() == GLFW_FALSE) fprintf(stderr, "Error, failed to init GLFW\n"); // glfwInit is the first one that needs to be called

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

    if (window == NULL) { fprintf(stderr, "Error, failed to create GLFW window\n"); glfwTerminate(); }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Vsync

    if (gl3wInit()) fprintf(stderr, "Error, failed to init Gl3w\n"); // gl3wInit needs to be called after a valid context (created window and made it current context) is created | Also this is the opengl loader
    
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Imgui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImFont* font1 = io.Fonts->AddFontFromFileTTF("Fonts/arial.ttf", 30);
    ImFont* font2 = io.Fonts->AddFontFromFileTTF("Fonts/consola.ttf", 30);

    // Style
    ImGui::StyleColorsDark();

    // Platform and Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    PerspectiveCamera pCamera = PerspectiveCamera(((float)WIN_WIDTH / WIN_HEIGHT), 90, {50,50,25});
    OrthographicCamera oCamera = OrthographicCamera( -200,200,-200,200,{0,0,50});

    RObjectManager::addCamera(&pCamera);
    RObjectManager::addCamera(&oCamera);

    DNDFileManager::setGLFWwindow(window);

    DNDImgWidget dndImgWidget = DNDImgWidget();
    DNDMosaicWidget dndMosaicWidget = DNDMosaicWidget();

    WSceneObject sceneObject = WSceneObject();

    bool wireframe = false;

    //glDisable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    WProperties properties = WProperties();
    WCamera wPCamera = WCamera(pCamera, "Perspective View");
    WCamera wOCamera = WCamera(oCamera, "Orthographic View");

    WRaytracing wRay = WRaytracing(pCamera);

    int screenshotsToOutputP = 0;
    int screenshotsToOutputO = 0;

    DirectionalLight();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.9, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // New frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        sceneObject.render();
        properties.render();
        wPCamera.render();
        wOCamera.render();
        wRay.render();

        MainWConsole::mainConsole.render();

        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("Windows"))
                {
                    if (ImGui::MenuItem("Console")) MainWConsole::mainConsole.p_open = !MainWConsole::mainConsole.p_open;
                    if (ImGui::MenuItem("Perspective")) wPCamera.p_open = !wPCamera.p_open;
                    if (ImGui::MenuItem("Orthographic")) wOCamera.p_open = !wOCamera.p_open;
                    if (ImGui::MenuItem("Properties")) properties.p_open = !properties.p_open;
                    if (ImGui::MenuItem("Scene graph")) sceneObject.p_open = !sceneObject.p_open;
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Primitives"))
                {
                    if (ImGui::MenuItem("Cube")) Cube();
                    if (ImGui::MenuItem("Pyramide")) Pyramide();

                    if (ImGui::MenuItem("Fleche")) Fleche();
                    if (ImGui::MenuItem("Plus Sign")) PlusSign();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Lights"))
                {
                    if (ImGui::MenuItem("Directional")) {
                        new DirectionalLight();
                    }
                    if (ImGui::MenuItem("Spot")) {
                        new SpotLight();
                    }
                    if (ImGui::MenuItem("Point")) {
                        new PointLight();
                    }

                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            {
                ImGui::Begin("Cameras controls");
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


                if (ImGui::Button("Wireframe")) {
                    if (!wireframe) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    }
                    else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                    }

                    wireframe = !wireframe;
                }

                /*if (ImGui::Button("Add console Text")) console.addLog( "This is a test", LogType::information);
                if (ImGui::Button("Add console Error")) console.addLog("This is an error", LogType::error);*/

                ImGui::Text("Perspective Camera");
                ImGui::Spacing();

                PerspectiveCamera* camera = (PerspectiveCamera*)RObjectManager::getCameras().at(0);

                {
                    float postitionData[4]{ camera->getPosition().x,camera->getPosition().y,camera->getPosition().z,camera->getFOV() };
                    float cPlaneData[2] = { camera->getNCP(), camera->getFCP() };
                    float lookAt[3]{ camera->getLookAt().x,camera->getLookAt().y,camera->getLookAt().z };

                    ImGui::Text("Camera X - Y - Z - FOV");
                    ImGui::DragFloat4("##C1-P", postitionData);
                    camera->setPosition({ postitionData[0],postitionData[1],postitionData[2] });
                    camera->setFOV(postitionData[3]);

                    ImGui::Text("Near/Far clipping plane");
                    ImGui::DragFloat2("##C1-CP", cPlaneData);
                    camera->setNCP(cPlaneData[0]);
                    camera->setFCP(cPlaneData[1]);

                    ImGui::Text("LookAt X - Y - Z");
                    ImGui::DragFloat3("##C1-LA", lookAt);
                    camera->setLookAt({ lookAt[0], lookAt[1], lookAt[2] });

                    ImGui::Text("Number of frame to Output : ");

                    ImGui::InputInt("##C1-SS", &screenshotsToOutputP, 1, 100);

                    if (screenshotsToOutputP < 0) {
                        screenshotsToOutputP = 0;
                    }

                    if (ImGui::Button("Output frames")) wPCamera.setImagesToOutput(screenshotsToOutputP);

                    ImGui::Spacing();

                    ImGui::Text("Background color : ");
                    float bColor[4]{ wPCamera.getClearColor().x*100, wPCamera.getClearColor().y*100, wPCamera.getClearColor().z*100, wPCamera.getClearColor().w*100 };

                    ImGui::DragFloat4("##C1-BGC", bColor);

                    wPCamera.setClearColor({ bColor[0]/100,bColor[1]/100,bColor[2]/100,bColor[3]/100 });
                }


                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Orthographic Camera");
                ImGui::Spacing();

                OrthographicCamera* camera2 = (OrthographicCamera*)RObjectManager::getCameras().at(1);

                {
                    float cornersPosition[4]{ camera2->getCorners()[0],camera2->getCorners()[1],camera2->getCorners()[2], camera2->getCorners()[3] };
                    float postitionData[3]{ camera2->getPosition().x,camera2->getPosition().y,camera2->getPosition().z };
                    float cPlaneData[2] = { camera2->getNCP(), camera2->getFCP() };
                    float lookAt[3]{ camera2->getLookAt().x,camera2->getLookAt().y,camera2->getLookAt().z };

                    ImGui::Text("Corners Left - Right - Bottom - Top");
                    ImGui::DragFloat4("##C2-C", cornersPosition);
                    camera2->setCorners(cornersPosition[0], cornersPosition[1], cornersPosition[2], cornersPosition[3]);

                    ImGui::Text("Camera X - Y - Z");
                    ImGui::DragFloat3("##C2-P", postitionData);
                    camera2->setPosition({ postitionData[0],postitionData[1],postitionData[2] });

                    ImGui::Text("Near/Far clipping plane");
                    ImGui::DragFloat2("##C2-CP", cPlaneData);
                    camera2->setNCP(cPlaneData[0]);
                    camera2->setFCP(cPlaneData[1]);

                    ImGui::Text("LookAt X - Y - Z");
                    ImGui::DragFloat3("##C2-LA", lookAt);
                    camera2->setLookAt({ lookAt[0], lookAt[1], lookAt[2] });

                    /*

                    ImGui::Text("Number of frame to Output : ");

                    ImGui::InputInt("##C2-SS", &screenshotsToOutputO, 1, 100);

                    if (screenshotsToOutputO < 0) {
                        screenshotsToOutputO = 0;
                    }

                    if (ImGui::Button("Output frames")) wOCamera.setImagesToOutput(screenshotsToOutputO);

                    ImGui::Spacing();
                    */


                    ImGui::Text("Background color : ");
                    float bColor[4]{ wOCamera.getClearColor().x*100, wOCamera.getClearColor().y*100, wOCamera.getClearColor().z*100, wOCamera.getClearColor().w*100 };

                    ImGui::DragFloat4("##C2-BGC", bColor);

                    wOCamera.setClearColor({ bColor[0]/100,bColor[1]/100,bColor[2]/100,bColor[3]/100 });
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Text("Raytracing View");
                ImGui::Spacing();

                if (ImGui::Button("Update Raytracing Image")) wRay.renderImage();
                ImGui::Spacing();
                if (ImGui::Button("Update Raytracing Meshes")) wRay.updateObjectMeshImage();

                ImGui::End();
            }
        }

        // Render dear imgui onto screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glfwSwapBuffers(window);

        DNDFileManager::clearPaths();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}