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
#include "SceneLoader.h"
#include "SceneParser_JSON.h"


int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    
    vec3 bgColor(args.bgColor[0], args.bgColor[1], args.bgColor[2]);
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(bgColor);
    

    std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(scene);
    SceneParser_JSON parser(loader);

    parser.parseFileData(args.inputFileName);


    // point3 eye = point3(0, 5,  2);
    // vec3 direction = vec3(0, -0.5, -1);
    // float focalLength = 1.0;
    // float imageplaneWidth = 1.0;

    // // by default set to 1,1,1
    // vec3 bgColor(args.bgColor[0], args.bgColor[1], args.bgColor[2]);
   
    // Framebuffer fb(args.width, args.height);

    // PerspectiveCamera persCam(fb.w(), fb.h(), eye, direction, imageplaneWidth, focalLength); 
    // std::shared_ptr<Scene> scene = std::make_shared<Scene>(bgColor);

    // //lights
    // //if you were trying to recreate my scene i forgot the light pos and colors i used while making my other scenes

    // std::shared_ptr<Light> l = std::make_shared<Light>(point3(0, 6, 2), vec3(1.0, 1.0, 1.0));
    // std::shared_ptr<Light> lTwo = std::make_shared<Light>(point3(5, 10, 0), vec3(0.8, 0.3, 0.6));

    // scene->pushLight(l);
    // scene->pushLight(lTwo);
    

    // // shaders
    // std::shared_ptr<BlinnPhong> bf = std::make_shared<BlinnPhong>(vec3(1, 1, 1), vec3(1, 1, 1), 128.0f);
    // std::shared_ptr<Lambert> lam = std::make_shared<Lambert>();
    // std::shared_ptr<Mirror> mi = std::make_shared<Mirror>();

    // // ground sphere 
    // scene->pushShape(std::make_shared<Sphere>(point3(0, -1000, -10), 995.0f, vec3(0.184, 0.929, 0.294), lam));
    

    // scene->pushShape(std::make_shared<Triangle>(point3(3,-1,-6),   point3(0,3,-6), point3(2.12,-1,-4.59), vec3(1.0, 0.2, 0.2), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(2.12,-1,-4.59), point3(0,3,-6), point3(0,-1,-4),   vec3(1.0, 0.6, 0.0), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(0,-1,-4),   point3(0,3,-6), point3(-2.12,-1,-4.59),vec3(1.0, 1.0, 0.0), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(-2.12,-1,-4.59), point3(0,3,-6), point3(-3,-1,-6), vec3(0.2, 0.8, 0.2), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(-3,-1,-6),  point3(0,3,-6), point3(-2.12,-1,-7.41),vec3(0.0, 0.6, 1.0), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(-2.12,-1,-7.41), point3(0,3,-6), point3(0,-1,-8),  vec3(0.4, 0.0, 1.0), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(0,-1,-8),   point3(0,3,-6), point3(2.12,-1,-7.41), vec3(0.8, 0.0, 0.8), lam));
    // scene->pushShape(std::make_shared<Triangle>(point3(2.12,-1,-7.41), point3(0,3,-6), point3(3,-1,-6),   vec3(1.0, 0.2, 0.5), lam));

    // scene->pushShape(std::make_shared<Sphere>(vec3(4,-3,-6), 1.0f, vec3(1,1,0), lam));
    // scene->pushShape(std::make_shared<Sphere>(vec3(-4,-3,-6), 1.0f, vec3(0,1,1), bf));
    // scene->pushShape(std::make_shared<Sphere>(vec3(0,-3,-5), 1.0f, vec3(1,0,0), lam));

    // scene->pushShape(std::make_shared<Sphere>(vec3(-3, 2,-8), 2.0f, vec3(0,1,0), mi));
    // scene->pushShape(std::make_shared<Sphere>(vec3(3, 2,-8), 2.0f, vec3(0,1,0), mi));
   
    // scene->pushShape(std::make_shared<Sphere>(vec3(15, 0,4), 6.0f, vec3(1,1,1), lam));
    // scene->pushShape(std::make_shared<Sphere>(vec3(15, 6, 4), 4.0f, vec3(1,1,1), lam));
    // scene->pushShape(std::make_shared<Sphere>(vec3(16, 10, 4), 3.0f, vec3(1,1,1), lam));
    
    
  

    RayRender r;
    Framebuffer fb(args.width, args.height);
    std::vector<std::shared_ptr<PerspectiveCamera>> pers = scene->getCameras();
    pers[0]->setDimensions(args.width, args.height);

    
    r.render(scene, fb, pers[0], args.rpp, args.recursionDepth);

    //export to png
    if(args.outputFileName.empty()) {
        fb.writeToPNG("render.png");
    } else {
        fb.writeToPNG(args.outputFileName + ".png");
    }
}