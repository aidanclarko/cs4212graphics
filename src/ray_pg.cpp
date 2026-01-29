#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

int main() {

    Framebuffer fb(100, 100);

    PerspectiveCamera p(fb.w(), fb.h()); 


    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {
            Ray r;
            p.generateRay(x, y, r);

            vec3 ray_dir_color = 0.5 * (vec3(1,1,1) + r.direction());

            fb.setPixelColor(y * fb.w() + x, ray_dir_color);
        }
    }

    //export to png
    png::image< png::rgb_pixel > pngOne( fb.w(), fb.h() );
    std::vector<vec3> storage = fb.getStorage();
    
    for (int y = 0; y < pngOne.get_height(); y++) {
        for (int x = 0; x < pngOne.get_width(); x++) {
            vec3 srgIndex = storage.at(y * fb.w() + x);
            float r = fb.floatToPngColor(srgIndex.x());
            float g = fb.floatToPngColor(srgIndex.y());
            float b = fb.floatToPngColor(srgIndex.z());

            pngOne[y][x] = png::rgb_pixel(r, g, b);
        }
    }

    pngOne.write( "ray.png" );


}