#include "handleGraphicsArgs.h"
#include "RayRender.h"
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
#include "PerspectiveCamera.h"


int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

     point3 eye = point3(0, 5, -1);
    vec3 direction = vec3(0, 0, -1);
    float focalLength = 1.0;
    float imageplaneWidth = 1.0;

    // by default set to 1,1,1
    vec3 bgColor(args.bgColor[0], args.bgColor[1], args.bgColor[2]);
   
    Framebuffer fb(args.width, args.height);

    PerspectiveCamera persCam(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(bgColor);

    //lights

    std::shared_ptr<Light> l = std::make_shared<Light>(point3(3, 12, -10), vec3(1,1,1));
    
    scene->pushLight(l);
    

    // shaders
    std::shared_ptr<BlinnPhong> bf = std::make_shared<BlinnPhong>(vec3(1, 1, 1), vec3(1, 1, 1), 128.0f);
    std::shared_ptr<Lambert> lam = std::make_shared<Lambert>();
    std::shared_ptr<Mirror> mi = std::make_shared<Mirror>();

    // ground sphere 
    
    scene->pushShape(std::make_shared<Sphere>(point3(0, 4, -25), 5, vec3(0.84, 0.21, 0.04), lam));
    
    
  

    RayRender r;
    r.render(scene, fb, persCam);

    //export to png
    fb.writeToPNG("withantialiasing.png");
}