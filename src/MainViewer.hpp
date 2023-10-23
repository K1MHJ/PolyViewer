#pragma once
#include "Geometry.hpp"
#include "Visual.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


using namespace Visual;
using namespace Geometry;

class MainViewer{
private:
  std::vector<std::tuple<Shape*, std::vector<Point>>> pShapes;
  std::vector<AREF> aref;
  
  VsRectangle* rectangle;
  VsAREF* visual_aref; 

  float left_um;
  float top_um;
  float width_um;
public:
  MainViewer(){
    left_um = 0;
    top_um = 0;
    width_um = 1000.0f;
  }
  ~MainViewer(){

  }
  void Initialize(){
    BuildShapes();
    MainViewer::Reorganize(pShapes, &aref);

    for(auto& ar : aref){
      std::string str = 
        string_format("Base(%f,%f) N(%d,%d) Col(%f,%f) Row(%f,%f) %d",
                    ar.base.X, ar.base.Y, 
                    ar.colN, ar.rowN, 
                    ar.colV.X, ar.colV.Y, 
                    ar.rowV.X, ar.rowV.Y,
                    ar.pShape->GetType());
      std::cout << str << std::endl;
    }
    rectangle = new VsRectangle(Rect(0,0,5,5));
    visual_aref = new VsAREF(Point(0,0), 100, 100, 10.0f, 10.0f, (VsObj*)rectangle);
  }
  void BuildShapes()
  {
    std::vector<Point> points;
    Rectangle* rect = new Rectangle();
    rect->Width(50.0);
    rect->Height(50.0);
    rect->Left(0);
    rect->Top(0);
    for(int i = 0;i<3;i++){
      for(int j=0;j<3;j++){
        points.push_back(Point(i*100.0,j*100.0)); 
      }
    }
    pShapes.push_back(std::make_tuple(rect, points));

    points.clear();
    rect = new Rectangle();
    rect->Width(50.0);
    rect->Height(50.0);
    rect->Left(0);
    rect->Top(0);
    for(int i = 0;i<3;i++){
      for(int j=0;j<3;j++){
        points.push_back(Point(i*200.0,j*200.0)); 
      }
    }
    pShapes.push_back(std::make_tuple(rect, points));
  }
  void OnRender(int wnd_width, int wnd_height){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Test");

    float height_um = width_um * (float)wnd_height / wnd_width;
    float dots_per_um;
    dots_per_um = wnd_height / width_um;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Wnd: %d, %d", wnd_width, wnd_height);

    visual_aref->OnRender(Rect(left_um, top_um, width_um, height_um), dots_per_um);

    ImGui::SliderFloat("Width[um]", &width_um, 10.0f, 10000.0f);
    ImGui::SliderFloat("Left", &left_um, -400.0f, 400.0f);
    ImGui::SliderFloat("Top", &top_um, -400.0f, 400.0f);


    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
  static void Reorganize(std::vector<std::tuple<Shape*, std::vector<Point>>>& p_vshapes, std::vector<AREF>* p_arefs)
  {
    std::vector<AREF>& arefs = *p_arefs;
    for(auto e : p_vshapes){
      auto& shape = std::get<0>(e);
      auto& points = std::get<1>(e);
      std::sort(std::get<1>(e).begin(), std::get<1>(e).end(), [](Point a, Point b){
        return a.Y < b.Y;
      });
      std::vector<std::vector<Point>> p;
      int i = 0;
      int j = 0;
      if(points.size() == 1){
        
      }
      else if(points.size() >= 2){
        while(i < points.size()-1){
          if(points[i].Y != points[i+1].Y){
            p.push_back(std::vector<Point>(points.begin()+j, points.begin()+i+1));
            j = i+1;
          } 
          i++;
        }
        if(j < i){
          p.push_back(std::vector<Point>(points.begin()+j, points.begin()+i+1));
        }
      }
      for(auto& a: p){
        std::sort(a.begin(), a.end(), [](Point a, Point b){
          return a.X < b.X;
        });
      }

      std::list<SREF> s;
      {
        std::list<SREF>::iterator itr = s.begin();
        for(auto& a:p){
          s.push_back(SREF());
          itr++;
          for(i = 0;i < a.size(); i++){
            if(i == 0){
              itr->base = a[0].X;
              itr->N = 1;
              itr->subValue = a[0].Y;
            }else{
              auto dx = a[i].X - a[i-1].X;
              if(itr->N == 1){
                itr->pitch = dx;
                itr->N++;
              }else{
                if(dx == itr->pitch){
                  itr->N++;
                }else{
                  s.push_back(SREF());
                  itr++;
                  itr->base = a[i].X;
                  itr->N = 1;
                  itr->subValue = a[i].Y;
                }
              }
            }
          }
        }
      }
      while(!s.empty()){
        bool first = false;
        for(auto itr = s.begin();itr != s.end();){
          if(!first){
            arefs.push_back(AREF());
            auto last = arefs.rbegin();
            last->base.X = itr->base;
            last->base.Y = itr->subValue;
            last->colN = itr->N;
            last->rowN = 1;
            last->colV.X = itr->pitch;
            last->colV.Y = 0;
            last->rowV.X = 0;
            last->rowV.Y = 0;
            first = true;
            itr = s.erase(itr);
          }else{
            auto last = arefs.rbegin();
            if(last->base.X == itr->base && last->colN == itr->N && last->colV.X == itr->pitch){
              auto dy = itr->subValue - (last->base.Y + last->rowV.Y * (last->rowN-1));
              if(last->rowN == 1){
                last->rowV.Y = dy;
                last->rowN++;
              }else{
                auto err = last->rowV.Y - dy;
                if(last->rowV.Y == dy){
                  last->rowN++;
                }else{
                  arefs.push_back(AREF());
                  auto last2 = arefs.rbegin();
                  last2->base.X = itr->base;
                  last2->base.Y = itr->subValue;
                  last2->colN = itr->N;
                  last2->rowN = 1;
                  last2->colV.X = itr->pitch;
                  last2->colV.Y = 0;
                  last2->rowV.X = 0;
                  last2->rowV.Y = 0;
                }
              }
              itr = s.erase(itr);
            }else{
              itr++;
            }
          }
        }
      }
      for(auto& ar : arefs){
        ar.pShape = shape;
      }
    }
  }
};
