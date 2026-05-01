#version 330 core

layout(location=0) in vec4 in_pos;
layout(location=1) in vec4 in_color;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float pointSize;

out vec4 color;
out float t;

void main() {
    gl_Position = projMatrix * viewMatrix * in_pos;
    gl_PointSize = pointSize;
    color = in_color;
}