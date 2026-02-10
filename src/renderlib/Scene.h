#pragma once
#include <vector>
#include "Shape.h"
#include "vec3.h"


class Scene {
    public:

        Scene() : bgColor( vec3(0,0,0)) {}
        Scene(vec3 bgColor) : bgColor(bgColor) {}

        const vec3& color() { return bgColor; }

        void pushShape(std::shared_ptr<Shape> s) {
            shapes.push_back(s);
        }

        void intersect(const Ray& r, std::shared_ptr<Shape>& sClosest, vec3& color, float& tmax, float& tmin) {
            for( const auto s : shapes ) {
                    if(s->intersect(r, tmin, tmax)) {
                        sClosest = s;
                    }
                }
                if(sClosest != nullptr) {
                    color = sClosest->getColor();
                }
        }

    private:
        std::vector<std::shared_ptr<Shape>> shapes;
        vec3 bgColor;
};