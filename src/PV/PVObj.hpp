#pragma once

namespace PV{
class PVObj{
public:
  const float Left(){return m_left;}
  const float Bottom(){return m_bottom;}
  const float Width(){return m_width;}
  const float Height(){return m_height;}
  const float Right(){return m_left + m_width;}
  const float Top(){return m_bottom + m_height;}
protected:
  float m_left;
  float m_bottom;
  float m_width;
  float m_height;

  PVObj(){};
  virtual ~PVObj(){};
};
}
