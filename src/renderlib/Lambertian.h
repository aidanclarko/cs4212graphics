#pragma once
#include "vec3.h"
#include "Shader.h"
#include "HitStruct.h"

class Lambert : public Shader {
    public:
        Lambert() {};

        vec3 rayColor(HitStruct& h) override { 
            vec3 hardCodedLight(0,20,5);
     
            vec3 ldir = unit_vector(hardCodedLight - h.point);
            
        
            float nDotl = std::fmax(0.0, dot(h.normal, ldir));
            vec3 lambertShade( nDotl, nDotl, nDotl );
            return lambertShade; 
        }
};