#version 330 core

in vec4 color;
out vec4 fragColor;
uniform sampler2D texUnit;
in float t;


void main() {
    float alpha = texture(texUnit, gl_PointCoord).r;

    fragColor = vec4(color.rgb, alpha);
}