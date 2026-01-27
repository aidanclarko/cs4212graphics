#include <vector>
#include "vec3.h"
#include <cmath>
#include "Framebuffer.h"

Framebuffer::Framebuffer(): width(100), height(100), fbStorage(100 * 100) { }
Framebuffer::Framebuffer(int w, int h): width(w), height(h), fbStorage(w * h) { }

double Framebuffer::floatToPngColor(float c) { return (std::round(c * 255)); }


void Framebuffer::setPixelColor(int i, vec3 color) {
    fbStorage[i] = color;
}

void Framebuffer::clearPNGToColor(vec3 color) {
    for(auto idx=0; idx < fbStorage.size(); idx++) {
        setPixelColor(idx, color);
    }
}

void Framebuffer::setPNGToSolidColor(vec3 color) {
    for(auto idx=0; idx < fbStorage.size(); idx++) {
        setPixelColor(idx, color);
    }
}

vec3 Framebuffer::findLERP(vec3 c1, vec3 c2, float t) {
    return (1 - t) * c1 + t * c2;
}

void Framebuffer::setPNGToLERP(vec3 c1, vec3 c2) {
    for (int y = 0; y < height; y++) {
        float t = static_cast<float>(y) / (height - 1);
        vec3 color = findLERP(c1, c2, t);

        for (int x = 0; x < width; x++) {
            setPixelColor(y * width + x, color);
        }
    }
}

void Framebuffer::setPNGCheckered(vec3 c1, vec3 c2) {
    for(int y = 0; y< height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            if((x + y) % 2 == 0) {
                setPixelColor(index, c1);
            } else {
                setPixelColor(index, c2);
            }
        }
    }
}
