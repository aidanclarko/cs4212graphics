#include <vector>
#include "vec3.h"
#include "png++/png.hpp"
#include <cmath>
#include "Framebuffer.h"

Framebuffer::Framebuffer(): width(100), height(100), fbStorage(100 * 100) { }
Framebuffer::Framebuffer(int w, int h): width(w), height(h), fbStorage(w * h) { }

double Framebuffer::floatToPngColor(float c) { return (std::round(c * 255)); }

//simple setter
void Framebuffer::setPixelColor(int i, vec3 color) {
    fbStorage[i] = color;
}

void Framebuffer::clearPNGToColor(vec3 color) {
    for(auto idx=0; idx < fbStorage.size(); idx++) {
        setPixelColor(idx, color);
    }
}


//clear
//arg single color, lerp, ...
// convert 0 to 1 float to 0 255

//export png
