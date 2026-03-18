#pragma once
#include <memory>
#include "vec3.h"
#include "Ray.h"
#include "Scene.h"
#include "Light.h"
#include "HitStruct.h"


class Shader {
    public:
        Shader() {}
        bool computeShadow(HitStruct& h, std::shared_ptr<Light> l);
        virtual vec3 rayColor(HitStruct& h,  std::vector<std::shared_ptr<Light>> lights, int depth) = 0;

    private:
        vec3 baseColor;
};