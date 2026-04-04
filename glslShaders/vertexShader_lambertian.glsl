#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec3 lightPos;

out vec4 normal;
out vec4 lightDir;

void main(void)
{
  normal = normalize(normalMatrix * vec4(in_Normal, 0.0));

  vec4 vertexWorldPos = modelMatrix * vec4(in_Position, 1.0);
  lightDir = normalize(vec4(lightPos, 1.0) - vertexWorldPos);

  gl_Position = projMatrix * viewMatrix * vertexWorldPos;
}