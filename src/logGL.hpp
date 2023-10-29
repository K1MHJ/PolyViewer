#pragma once

#define GLFW_NO_GLU
#define GLFW_INCLUDE_GL3
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl3ext.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <signal.h>
#include <memory>

#define ASSERT(x) if (!(x)) raise(SIGKILL);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

