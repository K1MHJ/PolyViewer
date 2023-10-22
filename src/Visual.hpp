#pragma once

#include "VertexBufferLayout.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "PositionBuffer.hpp"

#include "Geometry.hpp"
#include <memory>
#include <cassert>
#include <algorithm>
#include <functional>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include "glm/gtc/matrix_transform.hpp"

using namespace Geometry;

namespace Visual{
typedef unsigned char UInt8;
typedef unsigned short UInt16;
typedef unsigned int UInt32;
typedef char Int8;
typedef short Int16;
typedef int Int32;

class smallBmp{
public:
  smallBmp(UInt8 size){
    m_size = size;
    data = new UInt8[size*size];
  }
  ~smallBmp(){
    delete[] data;
  }
  UInt8 m_size;
public:
  UInt8* data;
};

class Rect{
public:
  Rect(){}
  Rect(float l, float t, float w, float h){
    left = l; top = t;width=w;height=h;
  }
  float left;
  float top;
  float width;
  float height;
};

class VsObj{
protected:
  VsObj(){}
public:
  virtual ~VsObj(){}
  virtual void OnRender(const Rect& view_range, float dots_per_um){}
public:
  Rect boundary;
  std::unique_ptr<VertexArray> m_VAO;
  std::unique_ptr<VertexBuffer> m_VertexBuffer;
  std::unique_ptr<IndexBuffer> m_IndexBuffer;
  std::unique_ptr<Shader> m_Shader;
};


class VsRectangle : public VsObj{
public:
  VsRectangle(Rect rc){
    float positions[] = {
      rc.left, rc.top                     , 0.0f, 0.0f, //0
      rc.left+rc.width, rc.top            , 1.0f, 0.0f, //1
      rc.left+rc.width, rc.top+rc.height  , 1.0f, 1.0f, //2
      rc.left, rc.top+rc.height           , 0.0f, 1.0f  //3
    };
    unsigned int indices[] = {
      0,1,2,
      2,3,0
    };
    m_VAO = std::make_unique<VertexArray>();
    m_VAO->Unbind();

    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
    m_VAO->Unbind();
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader = std::make_unique<Shader>("res/shaders/Rectangle.shader");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  }
  void OnRender(const Rect& mvp, float dots_per_um) override
  {
    m_MVP = glm::ortho(mvp.left, mvp.left + mvp.width, mvp.top , mvp.top + mvp.height, -1.0f, 1.0f);
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", m_MVP);
    m_Shader->Bind();
    m_VAO->Bind();
    m_IndexBuffer->Bind();
  }
  void OnRender(const Rect& mvp)
  {
    m_MVP = glm::ortho(mvp.left, mvp.left + mvp.width, mvp.top + mvp.height , mvp.top, -1.0f, 1.0f);

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", m_MVP);
    
    //draw
    m_Shader->Bind();
    m_VAO->Bind();
    m_IndexBuffer->Bind();
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
  }
  glm::mat4 m_MVP;
};


class VsAREF:public VsObj{
public:
  VsAREF(Point org, int colN, int rowN, float colP, float rowP, VsObj* obj){

    this->org = org;
    this->colN = colN;
    this->rowN = rowN;
    this->colP = colP;
    this->rowP = rowP;
    this->object = obj;

    positions = new glm::vec2[colN * rowN];
    int n = 0;
    for(int i=0;i<rowN;i++){
      for(int j=0;j<colN;j++){
        positions[n].x = org.X + colP * j;
        positions[n].y = org.Y + rowP * i;
        n++;
      }
    }
    // for(int i = 0;i<5;i++){
    //   bmp[i] = std::make_unique<smallBmp>((UInt8)2^i);
    // }

    object->m_VAO->Bind();

    object->m_VertexBuffer->Unbind();

    m_PositionBuffer = std::make_unique<PositionBuffer>((void*)positions, colN * rowN * sizeof(glm::vec2));
    m_PositionBuffer->Unbind(); 

    glEnableVertexAttribArray(0);
    object->m_VertexBuffer->Bind();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glEnableVertexAttribArray(1);
    m_PositionBuffer->Bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    
    glBindVertexArray(0);
  }
  ~VsAREF(){
    delete[] positions;
  }
  void OnRender(const Rect& view_range, float dots_per_um){
    float sizepxl = std::max(boundary.width, boundary.height) * dots_per_um;
    if(sizepxl <= 1.0){

    }else if(1.0 < sizepxl && sizepxl <= 2.0){
    }else if(2.0 < sizepxl && sizepxl <= 4.0){
    }else if(4.0 < sizepxl && sizepxl <= 8.0){
    }else if(8.0 < sizepxl && sizepxl <= 16.0){
    }else{
    }
    //draw instancing
    object->OnRender(view_range, dots_per_um);
    GLCall(glDrawElementsInstanced(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, NULL, colN * rowN));
  }
  void Init(){

  }
private:
  std::unique_ptr<smallBmp> bmp[5];
  Point org;
  int colN;
  int rowN;
  float colP;
  float rowP;
  VsObj* object; 
  std::unique_ptr<PositionBuffer> m_PositionBuffer;
  glm::vec2* positions;
};

}
