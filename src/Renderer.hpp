#pragma once

#include <GL/glew.h>
#include <signal.h>

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexBuffer.hpp"

#define ASSERT(x) if (!(x)) raise(SIGKILL);
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
  Renderer();
  ~Renderer();
  void Clear() const;
  void OnUpdate();
  void OnRender();

private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;
  glm::vec3 m_Translation;
  glm::mat4 m_Proj, m_View;


  unsigned int m_RendererID;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;
};
class Renderer2
{

public:
  Renderer2();
  ~Renderer2();
  void OnRender();

private:
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;
  glm::vec3 m_Translation;
  glm::mat4 m_Proj, m_View;


  unsigned int m_RendererID;
  unsigned char* m_LocalBuffer;
  int m_Width, m_Height, m_BPP;
};
