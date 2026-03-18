#pragma once
#include <memory>
#include "Shader.h"


class BlinnPhong : public Shader {
    public:
        BlinnPhong(vec3 d, vec3 s, float shin) : diff(d), spec(s), shininess(shin) {}
        
        vec3 rayColor(HitStruct& h, std::vector<std::shared_ptr<Light>> lights, int depth) override {
            vec3 total(0,0,0);

            for(auto l : lights) {
                vec3 viewDir = unit_vector(h.cameraPos - h.point);
                vec3 ldir = unit_vector( l->getPoint() - h.point );
                vec3 halfVec = unit_vector( viewDir + ldir);

                float nDotL = std::max(dot(h.normal, ldir), 0.0f);
                vec3 diffuse = diff * l->getColor() * nDotL;
                
                float nDotH = std::max(dot(h.normal, halfVec), 0.0f);
                vec3 specular = spec * l->getColor() * std::pow(nDotH, shininess);

                if(!computeShadow(h, l)) {
                    total +=  diffuse + specular;
                }
            }
            return total;
        }
    private:
        vec3 diff;
        vec3 spec;
        float shininess;

};