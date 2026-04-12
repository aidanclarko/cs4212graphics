#ifndef CAMERA_H
#define CAMERA_H
#include "vec3.h"
#include "Ray.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
            nx(pixel_nx), ny(pixel_ny), focalLength(1.0), imageplane_width(0.5) {
        
               float aspect_ratio = (float)nx / (float)ny;
               float height = imageplane_width / aspect_ratio;
               imageplane_height = aspect_ratio;
            }

            
        Camera(int pixel_nx, int pixel_ny, point3 eye, vec3 direction, 
                float imageplane_width, float focalLength) :
                pos(eye),
                W(unit_vector(-direction)),
                nx(pixel_nx), ny(pixel_ny),
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

                float aspect_ratio = (float)nx / (float)ny;
                imageplane_height = imageplane_width / aspect_ratio;
            }

            Camera(int pixel_nx, int pixel_ny, point3 eye, vec3 direction, 
                float imageplane_width, float focalLength, float fov, float aspect, float nearPlane, float farPlane) :
                pos(eye),
                W(unit_vector(-direction)),
                nx(pixel_nx), ny(pixel_ny),
                imageplane_width(imageplane_width), 
                focalLength(focalLength),
                fov(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane)
            {
                vec3 t;

                // if colinear ((dot) = 1 )then use a different basis for t
                t = vec3(0,1,0);

                if(fabs(dot(t, W)) > 0.999f) {
                    t = vec3(1,0,0);
                }
        
                U = unit_vector(cross(t, W));
                V = cross(W, U);

                float aspect_ratio = (float)nx / (float)ny;
                imageplane_height = imageplane_width / aspect_ratio;
            }


        ~Camera() {}

        virtual void generateRay(float i, float j, Ray &r) = 0;

        point3 getPos() const { return pos; }

        void setDimensions(int pixel_nx, int pixel_ny) {
            nx = pixel_nx;
            ny = pixel_ny;
            float aspect_ratio = (float)nx / (float)ny;
            imageplane_height = imageplane_width / aspect_ratio;
        }

         glm::mat4 getPerspectiveMatrix() const {
            return glm::perspective(fov, aspect,  nearPlane,  farPlane);
        }

         glm::mat4 lookAt() const {
            glm::vec3 gPos(pos.x(), pos.y(), pos.z());
            glm::vec3 gW(W.x(), W.y(), W.z());
            glm::vec3 gV(V.x(), V.y(), V.z());
            return glm::lookAt(gPos, gPos - gW, gV);
        }

        glm::vec3 getGLMPos() const {
            return glm::vec3(
                pos.x(),
                pos.y(),
                pos.z()
            );
        }

        void moveForward(float amount) { pos = pos + (-W) * amount; }
        void moveBack(float amount)    { pos = pos + W * amount; }
        void moveRight(float amount)   { pos = pos + U * amount; }
        void moveLeft(float amount)    { pos = pos - U * amount; }
        
        void rotateY(float amount) {
            glm::vec3 gW(W.x(), W.y(), W.z());
            glm::vec3 gV(V.x(), V.y(), V.z());
            glm::vec3 gU(U.x(), U.y(), U.z());
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0), amount, gV);

            gW = glm::vec3(rotation * glm::vec4(gW, 1.0));
            gU = glm::vec3(rotation * glm::vec4(gU, 1.0));
            this->W = vec3(gW.x, gW.y, gW.z);
            this->U = vec3(gU.x, gU.y, gU.z);
        }

        void rotateX(float amount) {
            glm::vec3 gW(W.x(), W.y(), W.z());
            glm::vec3 gV(V.x(), V.y(), V.z());
            glm::vec3 gU(U.x(), U.y(), U.z());
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0), amount, gU);

            gW = glm::vec3(rotation * glm::vec4(gW, 1.0));
            gU = glm::vec3(rotation * glm::vec4(gV, 1.0));
            this->W = vec3(gW.x, gW.y, gW.z);
            this->V = vec3(gV.x, gV.y, gV.z);
        }

        
    protected:
        vec3 pos;
        vec3 U,V,W;
        int nx, ny;
        float focalLength;
        float imageplane_width, imageplane_height;
        float fov, aspect, nearPlane, farPlane;
};

#endif

