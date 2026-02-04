#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

int main() {
    point3 eye = point3(3, -4, 12);
    vec3 direction = vec3(-8, -5, 10);
    float focalLength = 0.001;
    float imageplaneWidth = 0.5;
    float imageplaneHeight = 0.5;

    Framebuffer fb(200, 200);

    PerspectiveCamera p(fb.w(), fb.h()); 

    Sphere s(point3(0,0,0), 3.0f);

    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {
            Ray r;
            p.generateRay(x, y, r);

            vec3 ray_dir_color = 0.5 * (vec3(1,1,1) + r.direction());
            if(s.intersect(r)) {
                fb.setPixelColor(y * fb.w() + x, vec3(0.902, 0, 0.38));
            } else {
                fb.setPixelColor(y * fb.w() + x, ray_dir_color);
            }

            // fb.setPixelColor(y * fb.w() + x, ray_dir_color);
        }
    }

    //export to png
    png::image< png::rgb_pixel > pngOne( fb.w(), fb.h() );
    std::vector<vec3> storage = fb.getStorage();
    
    for (int y = 0; y < pngOne.get_width(); y++) {
        for (int x = 0; x < pngOne.get_height(); x++) {
            
            vec3 srgIndex = storage.at(y * fb.w() + x);
           
            float r = fb.floatToPngColor(srgIndex.x());
            float g = fb.floatToPngColor(srgIndex.y());
            float b = fb.floatToPngColor(srgIndex.z());

            pngOne[(pngOne.get_height() - 1) - y][x] = png::rgb_pixel(r, g, b);
        }
    }

    pngOne.write( "ray.png" );


}