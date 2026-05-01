#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPos;
uniform vec3 camPos;

out vec4 normal;
out vec3 halfVector;
out vec3 lightDir;
out vec2 texCoords;

void main(void)
{
  normal = normalize(normalMatrix * vec4(in_Normal, 0.0));

  vec4 vertexWorldPos = modelMatrix * vec4(in_Position, 1.0);

  vec3 viewDir = normalize(camPos - vertexWorldPos.xyz);
  lightDir = normalize(lightPos - vertexWorldPos.xyz);
  halfVector = normalize(viewDir + lightDir);
  texCoords = in_TexCoords;

  gl_Position = projMatrix * viewMatrix * vertexWorldPos;
}