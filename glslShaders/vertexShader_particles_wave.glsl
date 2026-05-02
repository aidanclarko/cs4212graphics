#version 330 core
layout(location=0) in vec4 in_pos;
layout(location=1) in vec4 in_color;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform float pointSize;
uniform float time;

out vec4 color;

void main() {
    vec4 p = in_pos;
    p.y = sin(p.x + time) * cos(p.z + time) * 5.0;
    gl_Position = projMatrix * viewMatrix * p;
    gl_PointSize = pointSize;
    color = in_color;
}