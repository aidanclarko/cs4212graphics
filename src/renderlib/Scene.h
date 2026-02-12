#pragma once
#include <vector>
#include "Shape.h"
#include "vec3.h"
#include "Shader.h"


class Scene {
    public:

        Scene() : bgColor( vec3(0,0,0)) {}
        Scene(vec3 bgColor) : bgColor(bgColor) {}

        const vec3& color() { return bgColor; }

        void pushShape(std::shared_ptr<Shape> s) {
            shapes.push_back(s);
        }


        //this should be computeRayColor();

        vec3 computeRayColor(const Ray& r, float tmin, float tmax, HitStruct& h) {
            bool hitShape = false;

            for(auto s : shapes) {
                if(s->intersect(r, tmin, tmax, h)) {
                    hitShape = true;
                }
            }

            if(hitShape) {
                return h.shader->rayColor(h);
                

            } else {
                return bgColor;
            }
           
        }

    private:
        std::vector<std::shared_ptr<Shape>> shapes;
        vec3 bgColor;
};