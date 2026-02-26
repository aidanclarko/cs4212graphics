#pragma once
#include <memory>
#include "vec3.h"
#include "Ray.h"
#include "Scene.h"
#include "Light.h"

struct HitStruct;


class Shader {
    public:
    //fake default
        Shader() {}
        
        bool computeShadow(HitStruct& h) {

            std::shared_ptr<Light> light = h.scene->getLight();
            
            vec3 ldir = unit_vector(light->getPoint() - h.point);
            float epsilon = 1e-4f;
            Ray shadow = Ray(h.point + vec3(epsilon, epsilon, epsilon), ldir);
            float distToLight = (light->getPoint() - h.point).length();
            HitStruct shadowHit;
            for(auto s : h.scene->getShapes()) {
                if(s->intersect(shadow, epsilon, distToLight, shadowHit)) {
                    return true;
                }
            }
            return false;
        }

        virtual vec3 rayColor(HitStruct& h,  std::shared_ptr<Light> l, int depth) = 0;

    private:
        vec3 baseColor;
};