#include <vector>
#include "vec3.h"
#include <cmath>
#include "Framebuffer.h"
#include "../png++/png.hpp"

Framebuffer::Framebuffer(): width(100), height(100), fbStorage(100 * 100) { }
Framebuffer::Framebuffer(int w, int h): width(w), height(h), fbStorage(w * h) { }

float Framebuffer::floatToPngColor(float c) { 
    c = std::max(0.0f, std::min(1.0f, c)); 
    return (c * 255.0f); 
}

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

void Framebuffer::writeToPNG(  std::string fileName ) {
    png::image< png::rgb_pixel > pngOne( width, height );
    std::vector<vec3> storage = getStorage();
    for (int y = 0; y < pngOne.get_height(); y++) {
        for (int x = 0; x < pngOne.get_width(); x++) {
            
            vec3 srgIndex = storage.at(y * width + x);
           
            float r = floatToPngColor(srgIndex.x());
            float g = floatToPngColor(srgIndex.y());
            float b = floatToPngColor(srgIndex.z());

            pngOne[(pngOne.get_height() - 1) - y][x] = png::rgb_pixel(r, g, b);
        }
    }

    pngOne.write( fileName );
}
