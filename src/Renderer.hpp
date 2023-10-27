#pragma once

#include "Shader.hpp"
#include <GL/glew.h>
#include <signal.h>
#include <memory>

#define ASSERT(x) if (!(x)) raise(SIGKILL);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
  std::unique_ptr<Shader> m_Shader;
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
public:
  void Initial();
  void Render();
  void Terminate();
};
