#pragma once
#include "vec3.h"

class Light {
    public:
        Light() {}
        Light(vec3 p) : point(p), color(vec3(1,1,1)) {}
        Light(vec3 p, vec3 c) : point(p), color(c) {}

        vec3 getPoint() { return point; }
        vec3 getColor() { return color; }

        void setPoint(vec3 p) {
            point = p;
        }
    private:
        point3 point;
        vec3 color;

};