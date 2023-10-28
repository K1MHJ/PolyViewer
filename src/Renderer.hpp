#pragma once

#include "Shader.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Renderer
{
  std::unique_ptr<Shader> m_Shader;
  unsigned int VBO;
  unsigned int VAO;
  unsigned int EBO;
  unsigned int texture[2];
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  std::vector<glm::vec3> cubePositions;
public:
  void Initial();
  void Render();
  void Terminate();
};
