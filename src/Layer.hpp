#pragma once

#include "logGL.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include <memory>

class Layer
{
  std::unique_ptr<Shader> m_Shader;    
  void OnAttach(){
      m_Shader = std::make_unique<Shader>("res/shaders/testbatch.shader");

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

      Renderer::Init();


  }
  void OnDetach()
  {
    Renderer::Shutdown();
  }
  void OnUpdate()
  {
    glClear(GL_COLOR_BUFFER_BIT);
    m_Shader->Bind();

    Renderer::ResetStats();
    Renderer::BeginBatch();
    
    for(float y = -10.0f;y<10.0f;y+=0.25f)
    {
      for(float x = -10.0f;x<10.0f;x+=0.25f)
      {
        glm::vec4 color = {(x+10)/20.0f, 0.2f, (y+10)/20.0f, 1.0f};
        Renderer::DrawQuad({x,y}, {0.25f, 0.25f}, color);
      }
    }
    Renderer::EndBatch();
  }
};
