#pragma once

#include <GL/glew.h>
#include <signal.h>
#include <memory>

#define ASSERT(x) if (!(x)) raise(SIGKILL);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

