#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mach/mach.h>
#include <signal.h>

#include "Renderer.hpp"
#include "MainViewer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


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
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 540, "PolyViewer", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);

  if(glewInit() != GLEW_OK){
    std::cout << "Error!" << std::endl;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  GLCall(glEnable(GL_BLEND));

  //Renderer2 renderer;
  MainViewer viewer;
  viewer.Initialize();
  int wnd_w, wnd_h;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    glfwGetWindowSize(window, &wnd_w, &wnd_h);
    viewer.OnRender(wnd_w, wnd_h);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
