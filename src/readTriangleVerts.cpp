#include <iostream>
#include <fstream>
#include <vector>
#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Scene.h"
#include "Lambertian.h"
#include "BlinnPhong.h"
#include "Mirror.h"
#include "Light.h"
#include "render_helpers.h"
#include "../png++/png.hpp"
#include "PerspectiveCamera.h"
#include <cmath>

void readFloatsFromFile(const std::string& filename, std::vector<float> &allFloats)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return;
    }

    float value;
    char c;

    while (file >> value) {
        allFloats.push_back(value);
        

        // Consume comma if there
        if (file.peek() == ',') file.get(c);
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

void render( std::shared_ptr<Scene> scene,  Framebuffer& fb, PerspectiveCamera& persCam ) {
    float rpp_NSquare = 4;
    vec3 c(0.0,0.0,0.0);

    for(int x= 0; x < fb.w(); x++) {
        for(int y = 0; y < fb.h(); y++) {

            //antialiasing
            for(int p = 0; p < rpp_NSquare; p++) {
                for(int q = 0; q < rpp_NSquare; q++) {

                    Ray r;
                    HitStruct h{ 
                        .cameraPos = persCam.getPos(),
                        .scene = scene
                    };

                    float pOffset = (p + random_float()) /rpp_NSquare;
                    float qOffset = (q + random_float()) /rpp_NSquare;

                    persCam.generateRay(x + pOffset , y + qOffset, r);
                    c += scene->computeRayColor(r, 1.0, INFINITY, h, 10);
                    

                }
            }
            c = c / (rpp_NSquare * rpp_NSquare);

            fb.setPixelColor(y * fb.w() + x,  c);
        }
    }
}


int main(int argc, char* argv[])
{
    std::vector<float> allFloats;
    readFloatsFromFile( "../src/trilist.dat", allFloats );
    
    std::cout << "Number of floats: " << allFloats.size() << std::endl;
    int numVertices = allFloats.size() / 3;
    std::cout << "Interpreted as Vertices: " << numVertices << std::endl;
    int numTriangles = numVertices / 3;
    std::cout << "Interpreted as Triangles: " << numTriangles << std::endl;


    point3 eye = point3(0, 0, 0);
    vec3 direction = vec3(0, 0, -1);
    float focalLength = 1.0;
    float imageplaneWidth = 1.0;
    vec3 bgColor(0.325, 0.659, 0.788);
    Framebuffer fb(200, 200);
    std::shared_ptr<Light> l = std::make_shared<Light>(point3(3, 12, 5), vec3(1,1,1));

    PerspectiveCamera persCam(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(bgColor, l);

    for(int i = 0; i < allFloats.size(); i+=9) {
        float a1 = allFloats[i];
        float a2 = allFloats[i + 1];
        float a3 = allFloats[i + 2];
        vec3 A(a1, a2, a3);

        float b1 = allFloats[i + 3];
        float b2 = allFloats[i + 4];
        float b3 = allFloats[i + 5];
        vec3 B(b1, b2, b3);

        float c1 = allFloats[i + 6];
        float c2 = allFloats[i + 7];
        float c3 = allFloats[i + 8];
        vec3 C(c1, c2, c3);
        

        std::shared_ptr<Triangle> t = std::make_shared<Triangle>(A, B, C);
        scene->pushShape(t);
    }

    // shaders
    std::shared_ptr<Lambert> lam = std::make_shared<Lambert>();

    // helper func above
    render(scene, fb, persCam);

    //export to png
    writeToPNG(fb, "triangleTest.png");
}
