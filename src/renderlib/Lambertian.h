#pragma once
#include <memory>
#include "vec3.h"
#include "Shader.h"
#include "HitStruct.h"
#include "Light.h"

class Lambert : public Shader {
    public:
        Lambert() {};
        Lambert(vec3 d) : diff(d) {};

        vec3 rayColor(HitStruct& h, std::vector<std::shared_ptr<Light>> lights, int depth) override { 
            
            vec3 hitPoint = h.point;
            vec3 lambertShade(0,0,0);
            
            for(auto l : lights) {
                vec3 ldir = unit_vector( l->getPoint() - h.point );
                float nDotl = std::fmax(0.0, ( dot(h.normal, ldir)));

                if(computeShadow(h)) {
                    lambertShade += diff * vec3(0.1, 0.1, 0.1);
                } else {
                    lambertShade += diff * l->getColor() * nDotl;
                }
            }
            return lambertShade; 
        }

    private:
        vec3 diff;
};