#version 330 core

layout(location=0) in vec4 in_pos;
layout(location=1) in vec4 in_color;
layout(location=2) in float in_life_span;
layout(location=3) in vec4 in_velocity;
layout(location=4) in vec4 in_gravity;
layout(location=5) in float in_index;
layout(location=6) in vec4 in_prevPos;


out vec4 out_pos;
out vec4 out_color;
out float out_life_span;
out vec4 out_velocity;
out vec4 out_gravity;
out float out_index;
out vec4 out_prevPos;

void main() {
    out_index = in_index;
    out_gravity = in_gravity;
    out_color = in_color;

    if(in_life_span > 0) {
        out_velocity = in_velocity + in_gravity;
        out_pos = in_pos + out_velocity;
        out_life_span = in_life_span - 0.005;
        out_prevPos = in_prevPos;
    } else {
        out_pos = in_prevPos;
        out_life_span = 0.5f;
        out_velocity = vec4(0.0);
        out_prevPos = in_prevPos;
    }
    
    gl_Position = vec4(0.0);
}

