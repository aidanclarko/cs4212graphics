#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Scene.h"
#include "render_helpers.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

void writeToPNG( Framebuffer& fb, std::string fileName ) {
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

    pngOne.write( fileName );
}


void render( Scene& scene,  Framebuffer& fb, PerspectiveCamera& p ) {
    float tmin = 1;
    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {
        
            float tmax = INFINITY;

            std::shared_ptr<Shape> shapeClosest = nullptr;
            vec3 color = scene.color();
            Ray r;

            p.generateRay(x  , y , r);
            scene.intersect(r, shapeClosest, color, tmax, tmin);

            fb.setPixelColor(y * fb.w() + x,  color);
        }
    }
}


int main() {
    point3 eye = point3(0,0,3);
    vec3 direction = vec3(0,0,-1);
    float focalLength = 1.0;
    float imageplaneWidth = 0.5;
    vec3 bgColor(0.325, 0.659, 0.788);
    Framebuffer fb(200, 200);
    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 


    Scene scene(bgColor);
    scene.pushShape(std::make_shared<Triangle>(point3(-1.2, -0.2, -7), point3(0.8, -0.5, -5), point3(0.9, 0, -5), vec3(1,0,0)));
    scene.pushShape(std::make_shared<Triangle>(point3(0.773205, -0.93923, -7), point3(0.0330127, 0.94282, -5), point3(-0.45, 0.779423, -5), vec3(0,1,1)));
    scene.pushShape(std::make_shared<Triangle>(point3(0.426795, 1.13923, -7), point3(-0.833013, -0.44282, -5), point3(-0.45, -0.779423, -5), vec3(0,0,1)));
    scene.pushShape(std::make_shared<Sphere>(point3(0, 0, -20), 5, vec3(0.5,0.5,0.23)));
    scene.pushShape(std::make_shared<Sphere>(point3(0, 0, -4), 0.05, vec3(0,0,0)));
    scene.pushShape(std::make_shared<Sphere>(point3(0, 0, -15), 1.2, vec3(1,1,1)));

    // helper func above
    render(scene, fb, p);

    //export to png
    writeToPNG(fb, "ray.png");
}