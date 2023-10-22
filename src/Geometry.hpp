#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <list>
#include <format>
#include <string>
#include <stdexcept>

namespace Geometry{
  template<typename ... Args>
  std::string string_format( const std::string& format, Args ... args )
  {
      int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
      if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
      auto size = static_cast<size_t>( size_s );
      std::unique_ptr<char[]> buf( new char[ size ] );
      std::snprintf( buf.get(), size, format.c_str(), args ... );
      return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
  }

  class Shape;

  class Point{
  public:
    Point(float x, float y){
      this->X=x;
      this->Y=y;
    }
    Point(){
      this->X = this->Y = 0;
    }
    Point(const Point &obj)
      : X(obj.X), Y(obj.Y)
    {
    }
    Point& operator = (const Point& rhs) {
      this->X = rhs.X;
      this->Y = rhs.Y;
      return *this;
    }
    float X;
    float Y;
  };
  class Vector{
  public:
    Vector(float x, float y){
      this->X=x;
      this->Y=y;
    }
    Vector(){
      this->X = 0;
      this->Y = 0;
    }
    Vector(const Vector &obj)
      : X(obj.X), Y(obj.Y)
    {
    }
    Vector& operator = (const Vector& rhs) {
      this->X = rhs.X;
      this->Y = rhs.Y;
      return *this;
    }
    float X;
    float Y;
  };
  class SREF{
  public:
    SREF(){
      base = 0;
      pitch = 0;
      N = 0;
    }
    float base;
    float subValue;
    float pitch;
    short N;
  };
  class AREF{
  public:
    AREF(){
      rowN = colN = 0;
      base = Point(0,0);
      colV = rowV = Vector(0,0);
    }
    Shape* pShape;
    Point base;
    Vector rowV, colV;
    short rowN,colN;
  };
  enum SHType{
    RECT,
    CIRCLE,
  };
  class Shape{
  protected:
    Shape(){
    }
    SHType type;
  public:
    float base_x;
    float base_y;
    const SHType GetType() const{
      return type;
    }
    virtual ~Shape(){
    }
  };
  class Circle : public Shape{
  public:
    Circle(float x, float y, float r){
      type = SHType::CIRCLE;
      this->x = x;
      this->y = y;
      this->radius = r;
      base_x = x;
      base_y = y;
    }
    void X(float x){
      this->x = x;
      this->base_x = x;
    }
    void Y(float y){
      this->y = y;
      this->base_y = y;
    }
    float x;
    float y;
    float radius;
    ~Circle(){

    }
  };
  class Rectangle : public Shape{
  public:
    Rectangle(){
      left = 0;
      top = 0;
      width = height = 0;
      type = SHType::RECT;
      base_x = left;
      base_y = top;
    }
    Rectangle(float l, float t, float w, float h){
      type = SHType::RECT;
      left = l;
      top = t;
      width = w;
      height = h;
      base_x = left;
      base_y = top;
    }
    ~Rectangle(){
    }
    void Left(float l){
      left = l;
      base_x = l;
    }
    void Top(float t){
      top = t;
      base_y = t;
    }
    void Width(float w){
      width = w;
    }
    void Height(float h){
      height = h;
    }
    const float GetLeft() const{
      return left;
    }
    const float GetTop() const{
      return top;
    }
    const float GetWidth() const{
      return width;
    }
    const float GetHeight() const{
      return height;
    }
  private:
    float left;
    float top;
    float width;
    float height;
  };
}
