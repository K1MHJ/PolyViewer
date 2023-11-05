#shader vertex
#version 330 core
layout(location = 0) in vec2 aPosVert;

uniform mat4 mvp;

void main()
{
  gl_Position = mvp * vec4(aPosVert, 0.0, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

uniform vec4 color;

void main()
{
  FragColor = color;
} 
