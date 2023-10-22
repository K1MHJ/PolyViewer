#shader vertex
#version 330 core

layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 offset;

uniform mat4 u_MVP;

void main()
{
  gl_Position = u_MVP * (vec4(vert, 0.0, 1.0) + vec4(offset, 0.0, 1.0));
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
  color = u_Color;
}
