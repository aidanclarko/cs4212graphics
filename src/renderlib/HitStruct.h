#pragma once
#include <memory>
#include "vec3.h"
#include "Ray.h"
class Shader;
class Shape;

struct HitStruct { 
    float t;
    Ray incomingRay;
    vec3 normal;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;
    vec3 point; 
    vec3 cameraPos;
    vec3 shapeColor;
};