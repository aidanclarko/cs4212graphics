#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
  public:
    Ray() : orig(point3(0,0,0)), dir(vec3(0,0,0)) {}

    Ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

     point3& origin()   { return orig; }
     vec3& direction()  { return dir; }

     

    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif