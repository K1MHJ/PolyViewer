#include <iostream>
#include "Renderer.hpp"
#include "Shader.hpp"
#include "logGL.hpp"
#include "PVRectangle.hpp"

float um_per_pxl;
float view_width_um;
float view_height_um;
float view_ofsX_um;
float view_ofsY_um;

std::unique_ptr<PV::PVRectangle> rect;
void Renderer::Initial()
{
  um_per_pxl = 1.0f;
  view_ofsX_um = 0.0f;
  view_ofsY_um = 0.0f;
  
  rect = std::make_unique<PV::PVRectangle>(50.0f, 50.0f, 50.0f, 100.0f);
}
void Renderer::Render(int width, int height)
{
  float aspect = (float)width / (float)height;
  view_width_um = width * um_per_pxl;
  view_height_um = height * um_per_pxl;

  glClearColor(0.5f, 0.5f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 projection = glm::ortho(view_ofsX_um, 
                          view_ofsX_um + view_width_um,
                          view_ofsY_um,
                          view_ofsY_um + view_height_um);
  glm::mat4 view(1.0f);
  glm::mat4 projView = projection * view;
  rect->Render(view_ofsX_um, view_ofsY_um, view_width_um, view_height_um, projView);
}
void Renderer::Terminate()
{
}

