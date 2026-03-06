#pragma once
#include <memory>
#include <vector>
#include "Shader.h"
#include "Shape.h"
#include "vec3.h"
#include "HitStruct.h"
#include "Light.h"
#include "PerspectiveCamera.h"


class Scene {
    public:
        Scene() : bgColor( vec3(0,0,0)) {}
        Scene(vec3 bgColor) : bgColor(bgColor) {}

        const vec3& color() { return bgColor; }

        void pushShape(std::shared_ptr<Shape> s) { shapes.push_back(s); }

        void pushLight(std::shared_ptr<Light> l) { lights.push_back(l); }

        void pushCamera(std::shared_ptr<PerspectiveCamera> p) { cameras.push_back(p); }

        std::vector<std::shared_ptr<Light>> getLights() { return lights; }

        std::vector<std::shared_ptr<Shape>> getShapes() { return shapes; }

        std::vector<std::shared_ptr<PerspectiveCamera>> getCameras() { return cameras; }


        vec3 computeRayColor(const Ray& r, float tmin, float tmax, HitStruct& h, int depth);

    private:
        std::vector<std::shared_ptr<PerspectiveCamera>> cameras;
        std::vector<std::shared_ptr<Shape>> shapes;
        std::vector<std::shared_ptr<Light>> lights;
        vec3 bgColor;
};