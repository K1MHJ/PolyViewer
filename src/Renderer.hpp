#pragma once

#include "Shader.hpp"
#include <GL/glew.h>

class Renderer
{
  std::unique_ptr<Shader> m_Shader;
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int texture;

public:
  void Initial();
  void Render();
  void Terminate();
};
