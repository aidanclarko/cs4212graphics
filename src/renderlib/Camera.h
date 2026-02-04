#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "Ray.h"

/*
    Goal: generate rays from camera --> turn ray directions into colors
*/
class Camera {
    public:
        //camera is at the main origin of the image plane
        Camera() : pos(0,0,0), U(1,0,0), V(0,-1,0), W(0,0,1), focalLength(1.0f), nx(100), ny(100), imageplane_height(0.5), imageplane_width(0.5) {}

        Camera(int pixel_nx, int pixel_ny) : 
            pos(0,0,0),
            U(1,0,0), V(0,-1,0), W(0,0,1), 
            nx(pixel_nx), ny(pixel_ny), focalLength(0.0001), imageplane_height(0.5), imageplane_width(0.5) {}
            
        Camera(int pixel_nx, int pixel_ny, point3 eye, vec3 direction, 
                float imageplane_height, float imageplane_width, float focalLength) :
                pos(eye),
                W(unit_vector(-direction)),
                nx(pixel_nx), ny(pixel_ny),
                imageplane_height(imageplane_height), 
                imageplane_width(imageplane_width),
                focalLength(focalLength) 
            {
                vec3 t;

                // if colinear ((dot) = 1 )then use a different basis for t
                t = vec3(0,1,0);

                if(fabs(dot(t, W)) > 0.999f) {
                    t = vec3(1,0,0);
                }
        
                U = unit_vector(cross(t, W));
                V = cross(W, U);
            }


        ~Camera() {}

        virtual void generateRay(int i, int j, Ray &r) = 0;
        
    protected:
        vec3 pos;
        vec3 U,V,W;
        int nx, ny;

        float focalLength;
        float imageplane_width, imageplane_height;
};

#endif

