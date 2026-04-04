#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec4 normal;
in vec3 lightDir;
in vec3 halfVector;

uniform float shininess;
uniform vec3 diffuse;
uniform vec3 specular;

void main(void)
{
  vec3 n = normalize(normal.xyz);
  vec3 h = normalize(halfVector);
  vec3 l = normalize(lightDir);

  float nDotL = max(dot(n, l), 0.0);
  float nDotH = max(dot(n, h), 0.0);

  vec3 diff = diffuse * nDotL;
  vec3 spec = specular * pow(nDotH, shininess);

  fragmentColor = vec4(diff + spec, 1.0);
}