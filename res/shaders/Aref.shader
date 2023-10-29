#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTransfer;

uniform mat4 view;

void main()
{
  gl_Position = view * vec4(aPos, 0.0, 1.0) * vec4(aTransfer, 0.0, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
  FragColor = color;
} 
