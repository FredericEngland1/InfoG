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

#include "RObject.h"

#include "PerspectiveCamera.h"
#include "Camera.h"

#include <opencv2/opencv.hpp>
#include "Renderer.h"


GLFWwindow* window;
GLFWmonitor* monitor;

const int WIN_HEIGHT = 1440;
const int WIN_WIDTH = 2560;
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

    MainWConsole::mainConsole.addLog(error, LogType::error);
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

    std::vector<glm::vec3> vertices{
        {-0.5,-0.5,-0.5},
        {-0.5,-0.5,0.5 },
        {0.5,-0.5,-0.5 },
        {0.5,-0.5,0.5 },
        {-0.5,0.5,-0.5 },
        {-0.5,0.5,0.5 },
        {0.5,0.5,-0.5 },
        {0.5,0.5,0.5 }
    };

    std::vector<unsigned int> indices{
        0,1,3,
        0,2,3,

        0,1,5,
        0,4,5,

        0,2,6,
        0,4,6,

        2,3,7,
        2,6,7,

        1,3,7,
        1,5,7,

        4,5,7,
        4,6,7
    };

    /*std::vector<glm::vec3> vertices{
        {-.5,.5,0},
        {.5,.5,0},
        {-.5,-.5,0}
    };

    std::vector<unsigned int> indices{
        0,1,2
    };*/

    RObject cube = RObject(vertices, indices, GL_TRIANGLES, "Cube");
    RObjectManager::addRObject(cube);

    PerspectiveCamera camera = PerspectiveCamera(((float)WIN_WIDTH / WIN_HEIGHT), 30, {15,10,4});

    Renderer renderer = Renderer(camera);


    DNDFileManager::setGLFWwindow(window);

    DNDImgWidget dndImgWidget = DNDImgWidget();
    DNDMosaicWidget dndMosaicWidget = DNDMosaicWidget();

    WSceneObject sceneObject = WSceneObject();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.9, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // New frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        renderer.render();
        sceneObject.render();

        MainWConsole::mainConsole.render();

        {
            ImGui::Begin("ScreenShot");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            if (ImGui::Button("Show Console")) MainWConsole::mainConsole.p_open = !MainWConsole::mainConsole.p_open;

            /*if (ImGui::Button("Add console Text")) console.addLog( "This is a test", LogType::information);
            if (ImGui::Button("Add console Error")) console.addLog("This is an error", LogType::error);*/

            if (ImGui::Button("Screenshot")) {

                cv::Mat image(1920, 1920, CV_8UC3);

                //use fast 4-byte alignment (default anyway) if possible
                glPixelStorei(GL_PACK_ALIGNMENT, (image.step & 3) ? 1 : 4);

                //set length of one complete row in destination data (doesn't need to equal img.cols)
                glPixelStorei(GL_PACK_ROW_LENGTH, image.step / image.elemSize());

                glReadPixels(0, 0, image.cols, image.rows, GL_BGR, GL_UNSIGNED_BYTE, image.data);

                //cv::cvtColor(img, outputImg, cv::COLOR_RGBA2BGRA);

                cv::Mat imageFlipper;

                cv::flip(image, imageFlipper, 0);

                cv::imwrite("result.jpg", imageFlipper);

            }

            ImGui::End();
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