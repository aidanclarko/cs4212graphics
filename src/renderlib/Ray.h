#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
  public:
    Ray() : orig(point3(0,0,0)), dir(vec3(0,0,0)) {}

    Ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

     const point3& origin() const { return orig;  }
     const vec3& direction() const { return dir; }

     void setDirection(const vec3 &d) {
        dir = d;
     }

     void setOrigin(const vec3 &o) {
      orig = o;
     }

     

    point3 at(double t) const {
        return orig + t*dir;
    }

  private:
    point3 orig;
    vec3 dir;
};

#endif