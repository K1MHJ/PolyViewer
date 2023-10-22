#include "PositionBuffer.hpp"
#include "Renderer.hpp"

PositionBuffer::PositionBuffer(const void* data, unsigned int size)
{
  GLCall(glGenBuffers(1, &m_RendererID));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
PositionBuffer::~PositionBuffer()
{
  GLCall(glDeleteBuffers(1, &m_RendererID));
}
void PositionBuffer::Bind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}
void PositionBuffer::Unbind() const
{
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
