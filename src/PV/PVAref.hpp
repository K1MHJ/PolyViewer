#pragma once

#include "PVRectangle.hpp"

namespace PV{
class PVAref:public PVObj{
  int nRow;
  int nColumn;
  float RowSpace;
  float ColumnSpace;
  float RefPointX;
  float RefPointY;
  std::unique_ptr<PVRectangle> rect;

  std::unique_ptr<Shader> shader;
  GLuint vbo; //vertex buffer
  GLuint ebo; //
  GLuint vao; //vertex array
public:
  PVAref(){
    nRow = 10;
    nColumn = 10;
    RowSpace = 20.0f;
    ColumnSpace = 20.0f;
    RefPointX = 0.0f;
    RefPointY = 0.0f;
    rect = std::make_unique<PVRectangle>(-5.0f, -5.0f, 10.0f, 10.0f);

    shader = std::make_unique<Shader>("res/shaders/Aref.shader");
    shader->Bind();
    shader->SetUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
    shader->Unbind();

    float right, left;
    float bottom, top;
    right = std::max(RefPointX, RefPointX + nColumn * ColumnSpace);
    left = std::min(RefPointX, RefPointX + nColumn * ColumnSpace);
    m_left = left + rect->Left();
    m_width = right + rect->Right() - m_left;

    top = std::max(RefPointY, RefPointY + nRow * RowSpace);
    bottom = std::min(RefPointY, RefPointY + nRow * RowSpace);
    m_bottom = bottom + rect->Bottom();
    m_height = top + rect->Top() - m_bottom;

    std::vector<glm::vec2> positions;
    for(int i = 0;i<nColumn;i++){
      for(int j = 0;j<nRow;j++){
        glm::vec2 p;
        p.x = RefPointX + ColumnSpace * i;
        p.y = RefPointY + RowSpace * j;
        positions.push_back(p);
      }
    }
    rect->Bind(positions);
  }
  ~PVAref(){
  }
  void Render(float fov_left, float fov_bottom, float fov_width, float fov_height,
              const glm::mat4 &projView){
    int n0 = (fov_left - rect->Right() - RefPointX) / ColumnSpace;
    int n1 = (fov_left + fov_width - RefPointX - rect->Left()) / ColumnSpace;

    int m0 = (fov_bottom - rect->Top() - RefPointY) / RowSpace;
    int m1 = (fov_bottom + fov_height - RefPointY - rect->Bottom()) / RowSpace;

    rect->Render(fov_left, fov_bottom, fov_width, fov_height, projView);
  }
};
}
