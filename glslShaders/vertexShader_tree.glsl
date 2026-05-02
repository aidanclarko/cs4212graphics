#version 330 core

layout(location=0) in vec2 offset;
layout(location=1) in vec2 uv;
layout(location=2) in vec4 treePos;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float pointSize;

out vec2 vUV;

void main() {
    vec3 right = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 up = vec3(0, 1, 0);
    vec3 worldPos = treePos.xyz + right * offset.x * pointSize + up * offset.y * pointSize;
    gl_Position = projMatrix * viewMatrix * vec4(worldPos, 1.0);
    vUV = uv;
}