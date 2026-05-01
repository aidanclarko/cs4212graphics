#version 330 core

layout(location = 0) in vec2 in_offset;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec4 in_pos;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float treeSize;

out vec2 texCoord;

void main() {
    vec3 right = vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    vec3 up = vec3(0.0, 1.0, 0.0);

    vec3 worldPos = in_pos.xyz + right * in_offset.x * treeSize + up * in_offset.y * treeSize;
    gl_Position = projMatrix * viewMatrix * vec4(worldPos, 1.0);
    
    texCoord = in_uv;
}