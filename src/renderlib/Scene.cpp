#include <memory>
#include <vector>
#include "Shader.h"
#include "Shape.h"
#include "vec3.h"
#include "HitStruct.h"
#include "Light.h"


vec3 Scene::computeRayColor(const Ray& r, float tmin, float tmax, HitStruct& h, int depth) {
    if(depth <= 0) return bgColor;
    bool hitShape = false;
    for(auto s : shapes) {
        if(s->intersect(r, tmin, tmax, h)) {
            h.shapeColor = s->getColor();
            hitShape = true;
        }
    }
    if(hitShape) {
        return h.shader->rayColor(h, light, depth - 1);
    }
    return bgColor;
}