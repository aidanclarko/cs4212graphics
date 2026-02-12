#pragma once
#include <vec3.h>
#include <Ray.h>
#include "Scene.h"

struct HitStruct;


class Shader {
    public:
    //fake default
        Shader() {}

        virtual vec3 rayColor(HitStruct& h) = 0;

    private:
        vec3 baseColor;
        
};