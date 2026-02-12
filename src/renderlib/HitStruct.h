#pragma once
#include "vec3.h"

class Shape;
class Shader;

struct HitStruct { 
    float t;
    vec3 normal;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;
    vec3 point; 
};