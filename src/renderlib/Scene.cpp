#include <memory>
#include <vector>
#include "Shader.h"
#include "Shape.h"
#include "vec3.h"
#include "HitStruct.h"
#include "Light.h"


bool Scene::computeShadow(HitStruct& h) {
    vec3 ldir = unit_vector(light->getPoint() - h.point);
    float epsilon = 0.001f;
    Ray shadow = Ray(h.point + epsilon * h.normal, ldir);
    float distToLight = (light->getPoint() - h.point).length();
    HitStruct shadowHit;
    for(auto s : shapes) {
        if(s->intersect(shadow, 0.0, distToLight, shadowHit)) {
            return true;
        }
    }
    return false;
}

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
        if(computeShadow(h)) {
            return h.shapeColor * vec3(0.1, 0.1, 0.1);
        }
        return h.shader->rayColor(h, light, depth - 1);
    }
    return bgColor;
}