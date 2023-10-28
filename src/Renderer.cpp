#include <iostream>
#include "Renderer.hpp"
#include "Shader.hpp"
#include "logGL.hpp"
#include "GLFW/glfw3.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

void Renderer::Initial()
{
  float vertices[] = {
       // 위치       // 컬러             // 텍스처 좌표
       0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
       0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
      -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
      -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 좌측 상단
  };
  unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
      0, 1, 3,   // 첫 번째 삼각형
      1, 2, 3    // 두 번째 삼각형
  };  
  cubePositions = {
    glm::vec3( 0.0f,  0.0f, 0.0f), 
    glm::vec3( 2.0f,  5.0f, 0.0f), 
    glm::vec3(-1.5f, -2.2f, 0.0f),  
    glm::vec3(-3.8f, -2.0f, 0.0f),  
    glm::vec3( 2.4f, -0.4f, 0.0f),  
    glm::vec3(-1.7f,  3.0f, 0.0f),  
    glm::vec3( 1.3f, -2.0f, 0.0f),  
    glm::vec3( 1.5f,  2.0f, 0.0f), 
    glm::vec3( 1.5f,  0.2f, 0.0f), 
    glm::vec3(-1.3f,  1.0f, 0.0f)  
  };
  model = glm::mat4(1.0f);
  view = glm::mat4(1.0f);
  projection = glm::mat4(1.0f);
  view       = glm::lookAt(
      glm::vec3(0,0,1), // ワールド空間でカメラは(4,3,3)にあります。
      glm::vec3(0,0,0), // 原点を見ています。
      glm::vec3(0,1,0)  // 頭が上方向(0,-1,0にセットすると上下逆転します。)
  );
  projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
  model = glm::translate(model, glm::vec3( 0.0f,  0.0f, 0.0f));

  m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
  m_Shader->Bind();
  m_Shader->SetUniform4f("ourColor", 1.0f, 0.0f, 0.0f, 1.0f);
  m_Shader->SetUniform1i("texture1", 0);
  m_Shader->SetUniform1i("texture2", 1);
  m_Shader->SetUniformMat4f("projection", projection);
  m_Shader->SetUniformMat4f("view", view);
  m_Shader->SetUniformMat4f("model", model);

  int width, height, nrChannels;
  unsigned char *data;
  for(int i = 0;i<2;i++){
    if(i == 0){
      data = stbi_load("./res/textures/dirt.png", &width, &height, &nrChannels, 0); 
    }
    if(i == 1){
      stbi_set_flip_vertically_on_load(true); 
      data = stbi_load("./res/textures/tile2.jpg", &width, &height, &nrChannels, 0); 
    }
    glGenTextures(1, &texture[i]);  
    glBindTexture(GL_TEXTURE_2D, texture[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(data){
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
  }


  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2); 

  glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0); 


}
void Renderer::Render()
{
  float timeValue = glfwGetTime();
  float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);

  m_Shader->Bind();
  m_Shader->SetUniform4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

  glBindVertexArray(VAO); 

  for(unsigned int i = 0; i < cubePositions.size(); i++)
  {
    model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
    float angle = 20.0f * i; 
    //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    //m_Shader->SetUniformMat4f("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBindVertexArray(0);
}
void Renderer::Terminate()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

