#version 330 core

in vec4 color;
out vec4 fragColor;
uniform sampler2D texUnit;
in float t;


void main() {
    float alpha = texture(texUnit, gl_PointCoord).r;

    vec3 hot  = vec3(1.0, 0.95, 0.6);  
    vec3 mid  = vec3(1.0, 0.2, 0.0);   
    vec3 dead = vec3(0.05, 0.0, 0.0);  

    vec3 color;

    if(t < 0.5) {
        color = mix(hot, mid, t * 2.0);
    } else {
        color = mix(mid, dead, t * 2.0);
    }
    color *= alpha;
    fragColor = vec4(color, alpha);
}