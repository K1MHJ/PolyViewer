#include <iostream>
#include "Renderer.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexBufferLayout.hpp"

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

Renderer::Renderer()
{
  m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
  m_View = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
  m_Translation = glm::vec3(200, 200, 0);
  m_Width = 400;
  m_Height = 400;

  float positions[] = {
    -50.0f, -50.0f, 0.0f, 0.0f, //0
     50.0f, -50.0f, 1.0f, 0.0f, //1
     50.0f,  50.0f, 1.0f, 1.0f, //2
    -50.0f,  50.0f, 0.0f, 1.0f  //3
  };
  
  unsigned int indices[] = {
    0,1,2,
    2,3,0
  };
  
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  
  m_VAO = std::make_unique<VertexArray>();

  m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  m_VAO->AddBuffer(*m_VertexBuffer, layout);

  m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

  m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
  m_Shader->Bind();
  m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  m_Shader->SetUniform1i("u_Texture", 0);

  m_LocalBuffer = new unsigned char[m_Width * m_Height * 4];
  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width,m_Height, 0,
                      GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));


  if(m_LocalBuffer)
    delete[] m_LocalBuffer;

}
Renderer::~Renderer()
{
  GLCall(glDeleteTextures(1, &m_RendererID));
}
void Renderer::Clear() const
{
}
void Renderer::OnUpdate()
{

}

void Renderer::OnRender()
{
  GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
  GLCall(glClear(GL_COLOR_BUFFER_BIT));

  Renderer renderer;

  GLCall(glActiveTexture(GL_TEXTURE0 + 0));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
  glm::mat4 mvp = m_Proj * m_View * model;
  m_Shader->Bind();
  m_Shader->SetUniformMat4f("u_MVP", mvp);
  m_Shader->Bind();

  m_VAO->Bind();
  m_IndexBuffer->Bind();
  GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
