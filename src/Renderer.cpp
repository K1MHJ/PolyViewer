#include <iostream>
#include "Renderer.hpp"
#include "Shader.hpp"

void GLClearError()
{
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
  while(GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

void Renderer::Initial()
{
  float vertices[] = {
       0.5f,  0.5f,  // 우측 상단
       0.5f, -0.5f,  // 우측 하단
      -0.5f, -0.5f,  // 좌측 하단
      -0.5f,  0.5f   // 좌측 상단
  };
  unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
      0, 1, 3,   // 첫 번째 삼각형
      1, 2, 3    // 두 번째 삼각형
  };  
  m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0); 
}
void Renderer::Render()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_Shader->Bind();
  glBindVertexArray(VAO); 

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  //glDrawArrays(GL_TRIANGLES, 0, 3);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBindVertexArray(0);
}
void Renderer::Terminate()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

