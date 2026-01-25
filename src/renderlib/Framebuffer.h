#pragma once

#include <vector>
#include "vec3.h"
#include "png++/png.hpp"
#include <cmath>

class Framebuffer {
    private:
        int width, height;
        std::vector<vec3> fbStorage;
    public:
        //default
        Framebuffer();
        Framebuffer(int w, int h);

        double floatToPngColor(float c);

        //simple setter
        void setPixelColor(int i, vec3 color);

        void clearPNGToColor(vec3 color);


        //clear
        //arg single color, lerp, ...
        // convert 0 to 1 float to 0 255

        //export png
};