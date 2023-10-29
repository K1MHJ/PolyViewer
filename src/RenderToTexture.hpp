#pragma once

#include <iostream>
#include "logGL.hpp"
#include "Shader.hpp"

bool RenderToTexture()
{
  // Create Frame Buffer
  GLuint frameBuffer = 0;
  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  // End

  // Create Texture
  int t_width,t_height;
  t_width = t_height = 200;
  GLuint renderedTexture;
  glGenTextures(1, &renderedTexture);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // End

  // Create Depth Buffer
  GLuint depthBuffer; 
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, t_width, t_height);  
  // End

  // Configure Frame Buffer
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0); 

  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if(fboStatus != GL_FRAMEBUFFER_COMPLETE){
  	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    return false;
  }
  // End
  
  // Render to the Texture
  GLint origFB;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &origFB);

  // Set frame buffer target & render
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
  glViewport(0,0,t_width,t_height);

  float vertices[] = {
       0.5f,  0.5f, 1.0f, 1.0f,   // 우측 상단
       0.5f, -0.5f, 1.0f, 0.0f,   // 우측 하단
      -0.5f, -0.5f, 0.0f, 0.0f,   // 좌측 하단
      -0.5f,  0.5f, 0.0f, 1.0f    // 좌측 상단
  };
  unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
      0, 1, 3,   // 첫 번째 삼각형
      1, 2, 3    // 두 번째 삼각형
  };  

  glm::mat4 projection = glm::mat4(1.0f);
  glm::mat4 view       = glm::lookAt(
      glm::vec3(0,0,1),
      glm::vec3(0,0,0),
      glm::vec3(0,1,0)
  );
  projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);

  std::unique_ptr<Shader> m_Shader;
  m_Shader = std::make_unique<Shader>("res/shaders/RenderToTex.shader");
  m_Shader->Bind();
  m_Shader->SetUniform1i("renderedTexture", 0);
  m_Shader->SetUniformMat4f("projection", projection);
  m_Shader->SetUniformMat4f("view", view);

  {
    int width, height, nrChannels;
    unsigned char *data;
    
    GLuint texture;
    glGenTextures(1, &texture);  
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("./res/textures/dirt.png", &width, &height, &nrChannels, 0); 
    if(data){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    GLuint EBO;
    GLuint VBO;
    GLuint VAO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    std::cout << __LINE__ << std::endl;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0.4f, 0.4f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    //glGenerateTextureMipmap(renderedTexture);
  }
  // End. Draw to frame buffer

  // Set frame buffer target to the back buffer
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  //glViewport(0,0,800,600);
  glClearColor(0.4f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);

  projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -0.0f, 1.0f);
  m_Shader->Bind();
  m_Shader->SetUniform1i("renderedTexture", 0);
  m_Shader->SetUniformMat4f("projection", projection);

  {
    GLuint EBO;
    GLuint VBO;
    GLuint VAO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_Shader->Bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
  }

  return true;
}
