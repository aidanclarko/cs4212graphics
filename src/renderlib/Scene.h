#pragma once
#include <vector>
#include "Shape.h"
#include "vec3.h"
#include "Shader.h"
#include "Light.h"


class Scene {
    public:

        Scene() : bgColor( vec3(0,0,0)) {}
        Scene(vec3 bgColor) : bgColor(bgColor) {}
        Scene(vec3 bgColor, std::shared_ptr<Light> l) : bgColor(bgColor), light(l) {}

        const vec3& color() { return bgColor; }

        void pushShape(std::shared_ptr<Shape> s) {
            shapes.push_back(s);
        }

        void pushLight(std::shared_ptr<Light> l) {
            lights.push_back(l);
        }


        //this should be computeRayColor();

        /*
            shadows:
                construct shadow ray <- POINT + t(LIGHT DIR - POINT)
                CHECK FOR INTERSECTION
                IF SO THEN DO SOME AMBIENT OCCLUSION TO COLOR
                OTHERWISE SHOULD BE NORMAL?

                SUBROUTINE

        
        
        */
       bool computeShadow(HitStruct& h) {
            vec3 ldir = unit_vector( light->getPoint() - h.point );
            float epsilon = 0.001f;
            Ray shadow = Ray(h.point + epsilon * h.normal, ldir);
            float distToLight = (light->getPoint() - h.point).length();
            HitStruct shadowHit;
            for(auto s : shapes) {
                if(s->intersect(shadow, 0.0, distToLight, shadowHit)) {
                    return true; 
                }

            }
            return false;
            
       }

        vec3 computeRayColor(const Ray& r, float tmin, float tmax, HitStruct& h) {
            bool hitShape = false;

            for(auto s : shapes) {
                if(s->intersect(r, tmin, tmax, h)) {
                    h.shapeColor = s->getColor();
                    hitShape = true;
                }
            }

            if(hitShape) {
                if(computeShadow(h)) {
                    return h.shapeColor * vec3(0.1, 0.1, 0.1);
                }
                return h.shader->rayColor(h, light);
                

            } else {
                return bgColor;
            }
        }

    private:
        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<std::shared_ptr<Light>> lights;
        std::shared_ptr<Light> light;
        vec3 bgColor;
};