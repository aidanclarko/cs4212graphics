#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Scene.h"
#include "Lambertian.h"
#include "Light.h"
#include "render_helpers.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

void writeToPNG( Framebuffer& fb, std::string fileName) {
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
            std::shared_ptr<Lambert> l = std::make_shared<Lambert>();

            Ray r;
            HitStruct h{  .shader = l };

            p.generateRay(x , y, r);
            vec3 color = scene.computeRayColor(r, 1.0, INFINITY, h);
            fb.setPixelColor(y * fb.w() + x,  color);
        }
    }
}


int main() {
    point3 eye = point3(0,0,0);
    vec3 direction = vec3(0,0,-1);
    float focalLength = 1.0;
    float imageplaneWidth = 0.5;
    vec3 bgColor(0.325, 0.659, 0.788);
    Framebuffer fb(600, 600);
    std::shared_ptr<Light> l = std::make_shared<Light>(point3(5, 10, -10), vec3(0.337, 0.588, 0.267));
    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 


    Scene scene(bgColor, l);
    scene.pushShape(std::make_shared<Sphere>(point3(1, 0, -20), 2.0f)); 

    scene.pushShape(std::make_shared<Sphere>(point3(-1, 0, -10), 0.25f)); 
    vec3 originalPos = l->getPoint(); 

    for(int i = 0; i < 37; i++) {
        float angle = i * 2.0f * M_PI / 36.0f;
        float x = originalPos.x() *  std::cos(angle) - originalPos.z() *  std::sin(angle);
        float z =  originalPos.x() *  std::sin(angle) + originalPos.z() *  std::cos(angle);

        std::cout << x << z << std::endl;
        std::cout << angle << "\n";
        
        l->setPoint(vec3(x, originalPos.y(), z));
        
        render(scene, fb, p);
        writeToPNG(fb, "ray" + std::to_string(i) + ".png");
    }
}