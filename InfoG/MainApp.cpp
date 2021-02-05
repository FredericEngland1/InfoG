#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "DNDImgWidget.h"
#include "DNDMosaicWidget.h"
#include "DNDFileManager.h"

GLFWwindow* window;
GLFWmonitor* monitor;

const int WIN_HEIGHT = 1080;
const int WIN_WIDTH = 1920;
const char *WIN_TITLE = "TestApp";

const char* glsl_version = "#version 460";

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

    fprintf(stderr, "GL_CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "* GL_ERROR *" : ""),
        type, severity, message);
}

int main()
{
    if (glfwInit() == GLFW_FALSE) fprintf(stderr, "Error, failed to init GLFW\n"); // glfwInit is the first one that needs to be called

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Version 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    
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

    // Style
    ImGui::StyleColorsDark();

    // Platform and Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // Hello triangle ?

    std::ifstream vShaderStream("RedVShader.shader");
    std::stringstream vBuffer;
    vBuffer << vShaderStream.rdbuf();

    std::ifstream fShaderStream("RedFShader.shader");
    std::stringstream fBuffer;
    fBuffer << fShaderStream.rdbuf();

    Shader baseShader = Shader(vBuffer.str(), fBuffer.str());

    // End of hello triangle one time actions

    DNDFileManager::setGLFWwindow(window);

    DNDImgWidget dndImgWidget = DNDImgWidget();
    DNDMosaicWidget dndMosaicWidget = DNDMosaicWidget();

    std::vector<float> vertices {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f
        /*-0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f*/
    };

    std::vector<unsigned int> indices {
        0, 1, 3,
        3, 2, 0
        //0, 1, 2
    };

    baseShader.use();

    unsigned int vao, vbo, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        // New frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Gui
        {
            ImGui::Begin("Demo window");
            ImGui::Button("Hello!");

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            dndImgWidget.display();
            dndMosaicWidget.display();

            ImGui::End();
        }

        // Draw triangle
        //baseShader.use();

        //std::cout << vertices.size() * sizeof(float) << std::endl;

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glBindVertexArray(0);

        // Render dear imgui onto screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glfwSwapBuffers(window);

        DNDFileManager::clearPaths();
    }

    //glDisableVertexAttribArray(0);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    baseShader.destroy();

    return 0;
}