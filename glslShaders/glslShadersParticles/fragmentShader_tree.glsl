#version 330 core

in vec2 vUV;

uniform sampler2D texUnitTree;

out vec4 fragColor;

void main() {
    vec4 color = texture(texUnitTree, vUV);
    if(color.a < 0.5) discard;
    fragColor = color;
}