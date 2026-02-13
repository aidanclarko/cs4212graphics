#pragma once


#include "vec3.h"
#include "Shader.h"
#include "HitStruct.h"
#include "Light.h"

class Lambert : public Shader {
    public:
        Lambert() {};

        vec3 rayColor(HitStruct& h, std::shared_ptr<Light> l) override { 
            vec3 lightPos = l->getPoint();
            vec3 hitPoint = h.point;
            
            
            vec3 ldir = unit_vector( l->getPoint() - h.point );
            /*
                huhu
            
            */

            
        
            float nDotl = std::fmax(0.0, ( dot(h.normal, ldir)));

            vec3 lambertShade = vec3( nDotl, nDotl, nDotl ) * l->getColor();
            return lambertShade; 
        }
};