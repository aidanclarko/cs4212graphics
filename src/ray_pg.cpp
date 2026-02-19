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

void fibboSphere(int n, Scene& s) {
    float goldenAngle = M_PI * (3.0f - sqrt(5.0f));
    float orbitRadius = 15.0f; 
    float sphereRadius = 0.5f;  

    for (int i = 0; i < n; i++) {
        float y = 1.0f - (i / float(n - 1)) * 2.0f;
        float r = sqrt(1.0f - y * y);
        float theta = goldenAngle * i;

        float x = cos(theta) * r * orbitRadius;
        float fz = sin(theta) * r * orbitRadius;
        float fy = y * orbitRadius;

        s.pushShape(std::make_shared<Sphere>(vec3(x, fy, fz), sphereRadius, vec3(1, 0, 0)));
    }
}

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


void render( Scene& scene,  Framebuffer& fb, PerspectiveCamera& persCam ) {
    float rpp_NSquare = 10;
    std::shared_ptr<BlinnPhong> l = std::make_shared<BlinnPhong>(vec3(1, 1, 1), vec3(1, 1, 1), 128.0f);
    std::shared_ptr<Lambert> lam = std::make_shared<Lambert>();
    vec3 c(0.0,0.0,0.0);

    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {

            //antialiasing
            for(int p = 0; p < rpp_NSquare; p++) {
                for(int q = 0; q < rpp_NSquare; q++) {

                    Ray r;
                    HitStruct h{ 
                        .shader = lam,
                        .cameraPos = persCam.getPos(),
                    };

                    float pOffset = (p + random_float()) /rpp_NSquare;
                    float qOffset = (q + random_float()) /rpp_NSquare;

                    persCam.generateRay(x + pOffset , y + qOffset, r);
                    c += scene.computeRayColor(r, 1.0, INFINITY, h);
                    

                }
            }
            c = c / (rpp_NSquare * rpp_NSquare);

            fb.setPixelColor(y * fb.w() + x,  c);
        }
    }
}


int main() {
    point3 eye = point3(0, 0, -5);
    vec3 direction = vec3(0, 0, -1);
    float focalLength = 1.0;
    float imageplaneWidth = 1.0;
    vec3 bgColor(0.325, 0.659, 0.788);
    Framebuffer fb(600, 600);
    std::shared_ptr<Light> l = std::make_shared<Light>(point3(10, 9, -5), vec3(1,1,1));
    PerspectiveCamera persCam(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    Scene scene(bgColor, l);

    // // ground sphere 
    scene.pushShape(std::make_shared<Sphere>(point3(0, -1000, -10), 995.0f, vec3(0.184, 0.929, 0.294)));

    scene.pushShape(std::make_shared<Sphere>(point3(0, -1, -25), 4, vec3(0.08, 0.91, 0.84)));

    scene.pushShape(std::make_shared<Sphere>(point3(2, 1, -15), 0.8f, vec3(1, 0.58, 0.157)));
    scene.pushShape(std::make_shared<Sphere>(point3(2.5, 2, -22), 1.8f, vec3(1, 0.58, 0.157)));

    scene.pushShape(std::make_shared<Sphere>(point3(-6, 3, -30), 1.0f, vec3(0.08, 0.91, 0.84)));
    scene.pushShape(std::make_shared<Sphere>(point3(7, 1, -32), 1.3f, vec3(1, 0.58, 0.157)));

    // fibboSphere(1000, scene);
    

    // helper func above
    render(scene, fb, persCam);

    //export to png
    writeToPNG(fb, "ray.png");
}