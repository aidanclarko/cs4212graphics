#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuse;

in vec4 normal;
in vec4 lightDir;

void main(void)
{
  float val = max(0.0, dot(normal, lightDir));
  vec3 diffuseShading = vec3( diffuse.r * val, diffuse.g * val, diffuse.b * val );

  fragmentColor = vec4(diffuseShading, 1.0);
}