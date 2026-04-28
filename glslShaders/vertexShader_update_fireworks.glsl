#version 330 core

layout(location=0) in vec4 in_pos;
layout(location=1) in vec4 in_color;
layout(location=2) in float in_life_span;
layout(location=3) in vec4 in_velocity;
layout(location=4) in vec4 in_gravity;
layout(location=5) in float in_index;
layout(location=6) in float in_expTime;
layout(location=7) in float in_fireworkTimer;

out vec4 out_pos;
out vec4 out_color;
out float out_life_span;
out vec4 out_velocity;
out vec4 out_gravity;
out float out_index;
out float out_expTime; 
out float out_fireworkTimer;

float rand(vec2 seed) {
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    out_index = in_index;
    out_gravity = in_gravity;
    out_color = in_color;
    float dt = 0.016;

    if(in_fireworkTimer > 0) {
        out_fireworkTimer = in_fireworkTimer - dt;
        out_pos = in_pos;
        out_velocity = in_velocity;
        out_life_span = in_life_span;
        out_expTime = in_expTime;

    } else {
        if(in_expTime > 0.0) {

        float delay = rand(vec2(in_index, 1.0)) * 0.008;
        float launchSpeed = 0.02 * (1 - delay);
        out_pos = in_pos + vec4(0.0f, launchSpeed, 0.0f, 0.0f);
        out_velocity = in_velocity;
        out_life_span = in_life_span;
        out_expTime = in_expTime - 0.005;

        } else if(in_life_span > 0.0) {

            out_velocity = in_velocity + in_gravity;
            out_pos = in_pos + out_velocity;
            out_life_span = in_life_span - dt;
            out_color = vec4(in_color.rgb, in_color.a - dt);
            out_expTime = 0.0;

        } else {

            out_velocity = in_velocity + in_gravity;
            out_pos = in_pos + out_velocity;
            out_life_span = 0.0;
            out_color = vec4(0.0, 0.0, 0.0, 0.0);
            out_expTime = 0.0;

        }
    }
    gl_Position = vec4(0.0);
}

