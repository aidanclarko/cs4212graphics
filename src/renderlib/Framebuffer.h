#pragma once

#include <vector>
#include "vec3.h"
#include <cmath>

class Framebuffer {
    private:
        int width, height;
        std::vector<vec3> fbStorage;
    public:
        //default
        Framebuffer();
        Framebuffer(int w, int h);
        int w() const {return width;}
        int h() const {return height;}
        std::vector<vec3> getStorage() {return fbStorage;}

        double floatToPngColor(float c);

        void setPixelColor(int i, vec3 color);

        void clearPNGToColor(vec3 color);

        void setPNGToSolidColor(vec3 color);

        //probably merge these two funcs
        vec3 findLERP(vec3 v1, vec3 v2, float t);

        void setPNGToLERP(vec3 v1, vec3 v2);

        void setPNGCheckered(vec3 c1, vec3 c2);
};