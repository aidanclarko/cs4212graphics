#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

/*
Then, Triangles.

Default Camera: focal length = 1.0, image plane width = 0.5

Triangle 1:
a: -1.2 -0.2 -7
b: 0.8 -0.5 -5
c: 0.9 0 -5

Triangle 2:
a: 0.773205 -0.93923 -7
b: 0.0330127 0.94282 -5
c: -0.45 0.779423 -5

Triangle 3:
a: 0.426795 1.13923 -7
b: -0.833013 -0.44282 -5
c: -0.45 -0.779423 -5

*/

int main() {
    point3 eye = point3(0,0,0);
    vec3 direction = vec3(0,0,-1);
    float focalLength = 1.0;
    float imageplaneWidth = 0.5;

    Framebuffer fb(200, 200);

    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 

    std::vector<std::shared_ptr<Shape>> shapes = {
        std::make_shared<Triangle>(point3(-1.2, -0.2, -7), point3(0.8, -0.5, -5), point3(0.9, 0, -5), vec3(1,0,0)),
        std::make_shared<Triangle>(point3(0.773205, -0.93923, -7), point3(0.0330127, 0.94282, -5), point3(-0.45, 0.779423, -5), vec3(0,1,1)),
        std::make_shared<Triangle>(point3(0.426795, 1.13923, -7), point3(-0.833013, -0.44282, -5), point3(-0.45, -0.779423, -5), vec3(0,0,1)),
    };

    float tmin = 1;
    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {
            std::shared_ptr<Shape> shapeClosest = nullptr;
            float tmax = INFINITY;
            vec3 color(1,1,1);
            Ray r;
            p.generateRay(x, y, r);

            for(const auto s : shapes) {
                if(s->intersect(r, tmin, tmax)) {
                    shapeClosest = s;
                    std::cout << "hit!" << "\n";
                }
            }
            if(shapeClosest != nullptr) {
                color = shapeClosest->getColor();
            }

            // NORMAL COLOR TEST
            // if(shapeClosest != nullptr) {
            //     vec3 N = unit_vector(r.at(tmax) - shapeClosest->getCenter());
            //     color = 0.5f* vec3(N.x() + 1, N.y() + 1, N.z() + 1);
            // }
           
            fb.setPixelColor(y * fb.w() + x, color);
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

            pngOne[(pngOne.get_height() - 1) - y][x] = png::rgb_pixel(r, g, b);
        }
    }

    pngOne.write( "ray.png" );


}