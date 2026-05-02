#version 330 core
layout(location=0) out vec4 fragmentColor;
uniform vec3 diffuse;
in vec4 normal;
in vec3 lightDir;
void main(void)
{
  float val = max(0.0, dot(normal.xyz, lightDir));
  fragmentColor = vec4(diffuse * val, 1.0);
}