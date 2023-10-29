#pragma once

#include "Shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Renderer
{
public:
  void Initial();
  void Render(int width, int height);
  void Terminate();
};
