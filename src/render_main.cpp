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
#include <chrono>


int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);
    
    vec3 bgColor(args.bgColor[0], args.bgColor[1], args.bgColor[2]);
    std::shared_ptr<Scene> scene = std::make_shared<Scene>(bgColor);
    

    std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(scene);
    SceneParser_JSON parser(loader);

    parser.parseFileData(args.inputFileName);

    if(!scene->getShapes().empty()) {
        scene->buildBVH(2);
    }

    RayRender r;
    Framebuffer fb(args.width, args.height);
    std::vector<std::shared_ptr<PerspectiveCamera>> pers = scene->getCameras();
    pers[0]->setDimensions(args.width, args.height);

    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    r.render(scene, fb, pers[0], args.rpp, args.recursionDepth);
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Code executed in: " << ms.count() << " milliseconds." << std::endl;


    //export to png
    if(args.outputFileName.empty()) {
        fb.writeToPNG("render.png");
    } else {
        fb.writeToPNG(args.outputFileName + ".png");
    }
}