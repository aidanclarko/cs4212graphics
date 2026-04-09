#pragma once;
#include "Triangle.h"

class GLMSphere {
    public:
        GLMSphere(float radius) : radius(radius) { initSquare(); }

        void subdivide(Triangle t, int depth, std::vector<Triangle>& out) {
            if(depth == 0) {
                out.push_back(t);
                return;
            }

            vec3 AB = (t.getA() + t.getB()) / 2.0;
            vec3 BC = (t.getB() + t.getC()) / 2.0;
            vec3 CA = (t.getC() + t.getA()) / 2.0;

            AB = unit_vector(AB); AB *= radius;
            BC = unit_vector(BC); BC *= radius;
            CA = unit_vector(CA); CA *= radius;

            subdivide(Triangle(t.getA(), AB, CA), depth - 1, out);
            subdivide(Triangle(AB, t.getB(), BC), depth - 1, out);
            subdivide(Triangle(CA, BC, t.getC()), depth - 1, out);
            subdivide(Triangle(AB, BC, CA), depth - 1, out);
        }

        std::vector<Triangle> makeMesh(int depth) {
            std::vector<Triangle> res;
            for(int i = 0; i < triList.size(); i++ ) {
                subdivide(triList[i], depth, res);
            }
            return res;
        }

    private:

        void initSquare() {
            vec3 v0 = unit_vector(vec3(-0.5, -0.5,  0.5)) * radius;
            vec3 v1 = unit_vector(vec3( 0.5, -0.5,  0.5)) * radius;
            vec3 v2 = unit_vector(vec3( 0.5,  0.5,  0.5)) * radius;
            vec3 v3 = unit_vector(vec3(-0.5,  0.5,  0.5)) * radius;
            vec3 v4 = unit_vector(vec3(-0.5, -0.5, -0.5)) * radius;
            vec3 v5 = unit_vector(vec3( 0.5, -0.5, -0.5)) * radius;
            vec3 v6 = unit_vector(vec3( 0.5,  0.5, -0.5)) * radius;
            vec3 v7 = unit_vector(vec3(-0.5,  0.5, -0.5)) * radius;

            triList = {
                Triangle(v0, v1, v3), Triangle(v1, v2, v3),
                Triangle(v1, v5, v2), Triangle(v5, v6, v2),
                Triangle(v4, v0, v7), Triangle(v0, v3, v7),
                Triangle(v5, v4, v6), Triangle(v4, v7, v6),
                Triangle(v3, v2, v7), Triangle(v2, v6, v7),
                Triangle(v4, v5, v0), Triangle(v5, v1, v0),
            };
        }
        std::vector<Triangle> triList;
        float radius;

};