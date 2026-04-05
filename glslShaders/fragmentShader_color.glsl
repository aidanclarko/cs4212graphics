#version 330 core

in vec3 color;
layout(location=0) out vec4 out_FragmentColor;

void main(void) {
    out_FragmentColor = vec4(color, 1.0);
}