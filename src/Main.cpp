#define GLFW_NO_GLU
#define GLFW_INCLUDE_GL3
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3ext.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mach/mach.h>
#include <signal.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (glfwInit() == GL_FALSE)
      return -1;

  // OpenGL Version 3.2 Core Profile を選択する
  // GL 3.2 + GLSL 150
  //const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 540, "OpenGL Tuto", NULL, NULL);
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
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  Renderer render;
  render.Initial();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Input */
    processInput(window);

    /* Render here */
    render.Render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }
  render.Terminate();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

