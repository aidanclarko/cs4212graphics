#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

int main() {
    point3 eye = point3(0,0,0);
    vec3 direction = vec3(0,0,-1);
    float focalLength = 1.0;
    float imageplaneWidth = 0.5;

    Framebuffer fb(600, 400);

    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 

    std::vector<std::shared_ptr<Shape>> shapes = {
        std::make_shared<Sphere>(point3(0,0,-10), 2.0f, vec3(0.23, 0.3, 1.0)),
        std::make_shared<Sphere>(point3(-1, 0, -8.5), 1.0f, vec3(1.0, 0.02, 0.45)),
        std::make_shared<Sphere>(point3(0, 1, -7), 0.5f, vec3(0.45, 0.32, 0.003)),
        std::make_shared<Sphere>(point3(-1, -1, -7), 0.25f, vec3(0, 0, 1.0)),
        std::make_shared<Sphere>(point3(-2, -1, -6), 0.4f, vec3(0, 0, 1.0)),
        std::make_shared<Sphere>(point3(1, -2, -5), 0.23f, vec3(0, 0, 1.0)),
        std::make_shared<Sphere>(point3(1, -2, -5), 2.3f, vec3(0, 0, 1.0)),
    };
    std::shared_ptr<Shape> s = std::make_shared<Sphere>( vec3(0,0,-10), 1.0);

    float tmin = 1;
    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {
            std::shared_ptr<Shape> shapeClosest = nullptr;
            float tmax = INFINITY;
            vec3 color(1,1,1);
            Ray r;
            p.generateRay(x, y, r);
            
            // // this is not actual code this was for Japan flag lab 3
            // if(s->intersect(r, tmin, tmax)) {
            //     std::cout << "hit" << "\n";
            //     color = vec3(1,0,0);
            // }

            // THIS IS ACTUAL CODE

            for(const auto s : shapes) {
                if(s->intersect(r, tmin, tmax)) {
                    shapeClosest = s;
                    std::cout << "hit!" << "\n";
                }
            }
            if(shapeClosest != nullptr) {
                vec3 N = unit_vector(r.at(tmax) - shapeClosest->getCenter());
                color = 0.5f* vec3(N.x() + 1, N.y() + 1, N.z() + 1);
            }
           
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