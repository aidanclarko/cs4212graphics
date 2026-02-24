#pragma once
#include <memory>
#include "Shader.h"


class BlinnPhong : public Shader {
    public:
        BlinnPhong(vec3 d, vec3 s, float shin) : diff(d), spec(s), shininess(shin) {}
        
        vec3 rayColor(HitStruct& h, std::shared_ptr<Light> l, int depth) override {
            vec3 viewDir = unit_vector(h.cameraPos - h.point);
            vec3 ldir = unit_vector( l->getPoint() - h.point );
            vec3 halfVec = unit_vector( viewDir + ldir);

            float nDotL = std::max(dot(h.normal, ldir), 0.0f);
            vec3 diffuse = h.shapeColor * diff * l->getColor() * nDotL;
            
            float nDotH = std::max(dot(h.normal, halfVec), 0.0f);
            vec3 specular = spec * l->getColor() * std::pow(nDotH, shininess);

            if(computeShadow(h)) {
                return h.shapeColor * vec3(0.1, 0.1, 0.1);
            } else {
                return diffuse + specular;
            }
        }
    private:
        vec3 diff;
        vec3 spec;
        float shininess;

};