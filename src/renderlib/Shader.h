#pragma once
#include <vec3.h>
#include <Ray.h>
#include "Scene.h"
#include "Light.h"

struct HitStruct;


class Shader {
    public:
    //fake default
        Shader() {}

        virtual vec3 rayColor(HitStruct& h,  std::shared_ptr<Light> l) = 0;

    private:
        vec3 baseColor;
        
};