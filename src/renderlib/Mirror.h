#pragma once
#include <memory>
#include "Shader.h"
#include "HitStruct.h"
#include "Ray.h"

class Mirror : public Shader {
    Scene& scene;
    public:
        Mirror(Scene& scene) : scene(scene) {}

        vec3 rayColor(HitStruct& h, std::shared_ptr<Light> l, int depth) override {
            vec3 d = h.incomingRay.direction();
            vec3 reflect = d - 2.0 * dot(d, h.normal) * h.normal;

            Ray reflected(h.point + 0.001f * h.normal, reflect);
            return scene.computeRayColor(reflected, 1.0, INFINITY, h,  depth);
        }

};