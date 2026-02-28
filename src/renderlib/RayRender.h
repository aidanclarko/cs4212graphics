#pragma once
#include <iostream>
#include "Framebuffer.h"
#include "vec3.h"
#include "Ray.h"
#include "Scene.h"
#include "Light.h"
#include "render_helpers.h"
#include "PerspectiveCamera.h"


class RayRender {

    public:
        RayRender() {}

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

};