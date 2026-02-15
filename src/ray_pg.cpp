#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Scene.h"
#include "Lambertian.h"
#include "BlinnPhong.h"
#include "Light.h"
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
    std::shared_ptr<BlinnPhong> l = std::make_shared<BlinnPhong>(vec3(0.3, 0.3, 0.8), vec3(1, 1, 1), 128.0f);
    std::shared_ptr<Lambert> lam = std::make_shared<Lambert>();
    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {

            Ray r;
            HitStruct h{ 
                .shader = lam,
                .cameraPos = p.getPos(),
            };

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
    Framebuffer fb(200, 200);
    std::shared_ptr<Light> l = std::make_shared<Light>(point3(5, 2, -5), vec3(0,1,1));
    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    Scene scene(bgColor, l);

    scene.pushShape(std::make_shared<Sphere>(point3(1,0,-20), 3.0f)); 
    scene.pushShape(std::make_shared<Sphere>(point3(-2, 1.5, -12), 1.0f)); 

    // helper func above
    render(scene, fb, p);

    //export to png
    writeToPNG(fb, "ray.png");
}