#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H
#include "Camera.h"
#include "Ray.h"

//lower focal length for coolness

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera() : Camera() {}
        PerspectiveCamera(float nx, float ny): Camera(nx, ny) {}
        PerspectiveCamera(float nx, float ny, point3 eye, vec3 direction, float imageplane_height, float imageplane_width, float focalLength) :
            Camera(nx, ny, eye, direction, imageplane_height, imageplane_width, focalLength) {
                std::cout << "W: " << W << "\nU: " << U << "\nV: " << V << std::endl;
            }

        void generateRay(int i, int j, Ray &ray) override {
            float u,v;
            float l,r,b,t;
            l = -imageplane_width/2;
            r = imageplane_width/2;
            b = -imageplane_height/2;
            t = imageplane_height/2;

            u = l + (r - l) * (i + 0.5)/nx;
            v = b + (t - b) * (j + 0.5)/ny;

            vec3 direction = unit_vector(-focalLength * W + u * U + v * V);
        

            ray.direction() = direction;
            ray.origin() = pos;
        }
};

#endif