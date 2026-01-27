#include <vector>
#include "renderlib/Framebuffer.h"
#include "renderlib/vec3.h"
#include "../png++/png.hpp"
#include <cmath>
#include "handleGraphicsArgs.h"

int main(int argc, char *argv[]) {
    // set framebuffer
    int width = 100;
    int height = 100;
    Framebuffer fb = Framebuffer(width, height);


    //this is for solid color

    vec3 solidColor = vec3(0.65, 0.439, 0.722);
    png::image< png::rgb_pixel > pngOne( fb.w(), fb.h() );
    
    fb.setPNGToSolidColor(solidColor);
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

    pngOne.write( "solid.png" );


    fb.clearPNGToColor(vec3(0,0,0));
    


    //for lerp
    vec3 c1 = vec3(0.588, 0.808, 1);
    vec3 c2 = vec3(1.0, 0.596, 0.325);

    png::image< png::rgb_pixel > pngTwo( fb.w(), fb.h() );

    //set to desired color float
    fb.setPNGToLERP(c1, c2);
    storage = fb.getStorage();

    for (int y = 0; y < pngTwo.get_height(); y++) {
        for (int x = 0; x < pngTwo.get_width(); x++) {
            vec3 srgIndex = storage.at(y * fb.w() + x);
            float r = fb.floatToPngColor(srgIndex.x());
            float g = fb.floatToPngColor(srgIndex.y());
            float b = fb.floatToPngColor(srgIndex.z());

            pngTwo[y][x] = png::rgb_pixel(r, g, b);
        }
    }
    
    pngTwo.write( "lerp.png" );

    c1 = vec3(0.929, 0.549, 0.969);
    c2 = vec3(0.282, 0.769, 0.463);

    png::image< png::rgb_pixel > pngThree( fb.w(), fb.h() );
    fb.setPNGCheckered(c1, c2);
    storage = fb.getStorage();

    for (int y = 0; y < pngThree.get_height(); y++) {
        for (int x = 0; x < pngThree.get_width(); x++) {
            vec3 srgIndex = storage.at(y * fb.w() + x);
            float r = fb.floatToPngColor(srgIndex.x());
            float g = fb.floatToPngColor(srgIndex.y());
            float b = fb.floatToPngColor(srgIndex.z());

            pngThree[y][x] = png::rgb_pixel(r, g, b);
        }
    }

    pngThree.write("checker.png");

}

// these are notes for future classes
/*

Shape Base class -> triange, circle inherit..

pure virtual class Shape (id do an interface) {
//factory class
    private:
        width, height, 
    public:
}

class Light {
    priv:
        intensity?
        hue?
    pub:
}

class Scene {
    list of shapes, cameras, whatever defines the scene
}

class Camera {
    priv:
    fov, basis,
    pub:
}

class Shader{}

*/