#include <iostream>
#include "logGL.hpp"
#include "Canvas.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void getFrameBufferSize(int *width, int *height);
float view_left, view_bottom;
float pxl_per_um;

CCanvas canvas;

int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (glfwInit() == GL_FALSE)
        return -1;
    // OpenGL Version 3.2 Core Profile を選択する
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "View", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if(glewInit() != GLEW_OK){
    std::cout << "Error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_BLEND);
    //glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    int wnd_w, wnd_h;
    view_left = 0.0f;
    view_bottom = 0.0f;
    pxl_per_um = 1.0f;

    canvas.Open();

    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        processInput(window);
        getFrameBufferSize( &wnd_w, &wnd_h);

        ImGui::Begin("Test");
        canvas.Render(wnd_w, wnd_h, view_left, view_bottom, pxl_per_um);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int wnd_w, int wnd_h)
{
    //canvas.Render(wnd_w, wnd_h, view_left, view_bottom, pxl_per_um);
}
void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}
void getFrameBufferSize(int *width, int *height)
{
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    *width = dims[2];
    *height = dims[3];
}