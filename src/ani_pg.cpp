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

/*

 ffmpeg -r 30 -f image2 -pattern_type glob -i "*.png" -vcodec libx264 -pix_fmt yuv420p output.mp4

*/

void setFibboShapes(int n, Scene& s, float oR) {
    float goldenAngle = M_PI * (3.0f - sqrt(5.0f));
    float orbitRadius = oR; 
    float sphereRadius = 0.6f;  

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
             std::shared_ptr<BlinnPhong> l = std::make_shared<BlinnPhong>(vec3(1, 1, 1), vec3(1, 1, 1), 128.0f);

            Ray r;
            HitStruct h{  .shader = l };

            p.generateRay(x , y, r);
            vec3 color = scene.computeRayColor(r, 1.0, INFINITY, h);
            fb.setPixelColor(y * fb.w() + x,  color);
        }
    }
}


int main() {
    point3 eye = point3(0, 0, 40);
    vec3 direction = vec3(0, 0, -1);
    float focalLength = 1.0;
    float imageplaneWidth = 1.0;
    vec3 bgColor(0.325, 0.659, 0.788);
    Framebuffer fb(400, 400);
    std::shared_ptr<Light> l = std::make_shared<Light>(point3(10, 9, -5), vec3(1,1,1));
    PerspectiveCamera p(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    Scene scene(bgColor, l);

    // // ground sphere 
    // scene.pushShape(std::make_shared<Sphere>(point3(0, -1000, -10), 995.0f, vec3(0.184, 0.929, 0.294)));
    
    // scene.pushShape(std::make_shared<Sphere>(point3(0, -1, -25), 4, vec3(0.08, 0.91, 0.84)));
    
    // scene.pushShape(std::make_shared<Sphere>(point3(2, 1, -15), 0.8f, vec3(1, 0.58, 0.157)));
    // scene.pushShape(std::make_shared<Sphere>(point3(2.5, 2, -22), 1.8f, vec3(1, 0.58, 0.157)));
    
    // scene.pushShape(std::make_shared<Sphere>(point3(-6, 3, -30), 1.0f, vec3(0.08, 0.91, 0.84)));
    // scene.pushShape(std::make_shared<Sphere>(point3(7, 1, -32), 1.3f, vec3(1, 0.58, 0.157)));

    vec3 originalPos = l->getPoint(); 

    for(int i = 0; i < 10; i++) {
        setFibboShapes(50, scene, i);
        
        render(scene, fb, p);

        std::string filename;
        if(i < 10) {
            filename = "ray00" + std::to_string(i) + ".png";
        } else if(i < 100) {
            filename = "ray0" + std::to_string(i) + ".png";
        } else {
            filename = "ray" + std::to_string(i) + ".png";
        }
        writeToPNG(fb, filename);
        }
}