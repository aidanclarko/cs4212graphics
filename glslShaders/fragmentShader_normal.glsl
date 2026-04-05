#version 330 core

layout(location=0) out vec4 fragmentColor;


in vec4 normal;

void main(void)
{
  float r = (normal.x + 1) * 0.5;
  float g = (normal.y + 1) * 0.5;
  float b = (normal.z + 1) * 0.5;


  fragmentColor = vec4(r, g, b, 1.0);
}