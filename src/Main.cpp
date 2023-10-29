
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mach/mach.h>
#include <signal.h>

#include "logGL.hpp"

#include "Renderer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  float aspect = (float)width / (float)height;
  glViewport(0, 0, width, height);
  // glEnable(GL_TEXTURE_2D);
  // glEnable(GL_BLEND); //Enable alpha blending
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glClearColor(0.0, 0.0, 0.0, 1.0);
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // gluOrtho2D(0.0, width, height * aspect, 0.0);
  // glMatrixMode(GL_MODELVIEW);
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

  int width, height;

  Renderer render;
  render.Initial();

  glfwSwapBuffers(window);
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Input */
    processInput(window);

    glfwGetWindowSize(window, &width, &height);
    /* Render here */
    render.Render(width, height);

    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }
  // render.Terminate();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}

