#pragma once
#include <memory>
#include "Shader.h"
#include "HitStruct.h"
#include "Ray.h"

class Mirror : public Shader {

    public:
        Mirror() {}

        vec3 rayColor(HitStruct& h, std::vector<std::shared_ptr<Light>> lights, int depth) override {
            vec3 d = h.incomingRay.direction();
            vec3 reflect = d - 2.0 * dot(d, h.normal) * h.normal;

            Ray reflected(h.point + 0.01f * h.normal, reflect);
        
            return h.scene->computeRayColor(reflected, 0.01, INFINITY, h,  depth);
        }
};