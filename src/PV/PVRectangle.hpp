#pragma once

#include "PVObj.hpp"
#include "logGL.hpp"
#include "Shader.hpp"
#include <memory>
#include <iostream>

namespace PV{

class PVRectangle : public PVObj{
  std::unique_ptr<Shader> shader;
  GLuint vbo;
  GLuint ebo;
  GLuint vao;
  GLuint pbo;
  int count_instance;
public:
  PVRectangle(float left, float bottom, float width, float height){
    m_left = left;
    m_bottom = bottom;
    m_width = width;
    m_height = height;
    float vertices[] = {
       left+width, bottom+height,
       left+width, bottom,
       left, bottom, 
       left, bottom+height
    };
    unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
        0, 1, 3,   // 첫 번째 삼각형
        1, 2, 3    // 두 번째 삼각형
    };  
    float pos[] = {
      0,0,200,0
    };

    shader = std::make_unique<Shader>("res/shaders/Rectangle.shader");
    shader->Bind();
    shader->SetUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
    shader->Unbind();

    glGenBuffers(1, &ebo); 
    glGenBuffers(1, &vbo); 
    glGenBuffers(1, &pbo); 
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, pbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    count_instance = sizeof(pos) / sizeof(float) / 2;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, pbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glVertexAttribDivisor(0,0);
    glVertexAttribDivisor(1,1);

    glBindVertexArray(0);
  }
  void Bind(const std::vector<glm::vec2> &positions){
    count_instance = positions.size();
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, pbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), &positions.front(), GL_STATIC_DRAW);
    glBindVertexArray(0);
  }
  ~PVRectangle(){
    glDeleteBuffers(1, &pbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
  }
  void Render(float fov_left, float fov_bottom, float fov_width, float fov_height,
              const glm::mat4 &projView)
  {
    glBindVertexArray(vao);
    shader->Bind();
    shader->SetUniformMat4f("view", projView);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElementsInstanced(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, NULL, count_instance);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
};
}
