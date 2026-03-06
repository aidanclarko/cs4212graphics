#ifndef PERSPECTIVE_CAMERA_H
#define PERSPECTIVE_CAMERA_H
#include "Camera.h"
#include "Ray.h"

class PerspectiveCamera : public Camera {
    public:
        PerspectiveCamera() : Camera() {}
        PerspectiveCamera(float nx, float ny): Camera(nx, ny) {}
        PerspectiveCamera(float nx, float ny, point3 eye, vec3 direction, float imageplane_width, float focalLength) :
            Camera(nx, ny, eye, direction, imageplane_width, focalLength) {}

        PerspectiveCamera(point3 eye, vec3 direction, float imageplane_width, float focalLength)
            : Camera(0, 0, eye, direction, imageplane_width, focalLength) {}

        void generateRay(float i, float j, Ray &ray) override {
            float u,v;
            float l,r,b,t;
            l = -imageplane_width/2;
            r = imageplane_width/2;
            b = -imageplane_height/2;
            t = imageplane_height/2;

            u = l + (r - l) * (i)/nx;
            v = b + (t - b) * (j)/ny;

            vec3 direction = unit_vector(-focalLength * W + u * U + v * V);
        
            ray.setDirection(direction);
            ray.setOrigin(pos);
        }

};

#endif