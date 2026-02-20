#pragma once
#include <memory>
#include "vec3.h"
#include "Shader.h"
#include "HitStruct.h"
#include "Light.h"

class Lambert : public Shader {
    public:
        Lambert() {};

        vec3 rayColor(HitStruct& h, std::shared_ptr<Light> l, int depth) override { 
            vec3 hitPoint = h.point;
        
            vec3 ldir = unit_vector( l->getPoint() - h.point );
           
            float nDotl = std::fmax(0.0, ( dot(h.normal, ldir)));

            vec3 lambertShade = h.shapeColor * l->getColor() * nDotl;
            return lambertShade; 
        }
};