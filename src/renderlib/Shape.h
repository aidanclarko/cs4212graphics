#ifndef SHAPE_H
#define SHAPE_H

#include "Ray.h"

//base class
class Shape {
    public:
        virtual bool intersect( const Ray& r, const float tmin, float &tmax) = 0;
        virtual const vec3& getColor() const = 0;
        virtual const point3& getCenter() const = 0;
    private:
        vec3 color;
};

#endif