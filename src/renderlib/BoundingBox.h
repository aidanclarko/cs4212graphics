#pragma once

#include "vec3.h"
#include "Ray.h"
#include <cfloat>

class BoundingBox  {
    public:
        vec3 llb;
        vec3 urf;

        //def
        BoundingBox() : 
            llb(vec3(FLT_MAX, FLT_MAX, FLT_MAX)), 
            urf(vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX)) {}

        BoundingBox(vec3 lowerLeft, vec3 upperRight) : llb(lowerLeft), urf(upperRight) {}

        void update(vec3 v) {
            for(int i = 0; i < 3; i++) {
                if(v[i] < llb[i]) {
                    llb[i] = v[i];
                }

                if(v[i] > urf[i]) {
                    urf[i] = v[i];
                }
            }
        }

        bool hit(const Ray &r, float tmin, float tmax) const {
            for (int axis = 0; axis < 3; axis++) {
                float t0, t1;
                if (r.direction()[axis] >= 0) {
                    t0 = (llb[axis] - r.origin()[axis]) / r.direction()[axis];
                    t1 = (urf[axis] - r.origin()[axis]) / r.direction()[axis];
                } else {
                    t0 = (urf[axis] - r.origin()[axis]) / r.direction()[axis];
                    t1 = (llb[axis] - r.origin()[axis]) / r.direction()[axis];
                }
                if (t0 > tmin) tmin = t0;
                if (t1 < tmax) tmax = t1;
                if (tmax < tmin) return false;
            }
            return true;
    }

};