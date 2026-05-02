#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec4 normal;
in vec3 lightDir;
in vec3 halfVector;
in vec2 texCoords;

uniform float shininess;
uniform vec3 diffuse;
uniform vec3 specular;
uniform sampler2D texUnit;

void main(void)
{
  vec3 n = normalize(normal.xyz);
  vec3 h = normalize(halfVector);
  vec3 l = normalize(lightDir);

  float nDotL = max(dot(n, l), 0.0);
  float nDotH = max(dot(n, h), 0.0);
  vec3 diffuseTex = texture(texUnit, texCoords).rgb;

  vec3 diff = diffuseTex * nDotL;
  vec3 spec = specular * pow(nDotH, shininess);

  fragmentColor = vec4(diff + spec, 1.0);
}