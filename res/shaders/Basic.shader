#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexColor; 
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
  vertexColor = aColor;
  TexCoord = aTexCoord;
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 ourColor;

void main()
{
  FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5) * vec4(vertexColor, 1.0) * ourColor;
} 
