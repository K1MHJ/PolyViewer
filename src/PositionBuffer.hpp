#pragma once

class PositionBuffer
{
private:
  unsigned int m_RendererID;
public:
  PositionBuffer(const void* data, unsigned int size);
  ~PositionBuffer();

  void Bind() const;
  void Unbind() const;
};
