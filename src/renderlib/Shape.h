#ifndef SHAPE_H
#define SHAPE_H

#include "Ray.h"
#include "HitStruct.h"
#include "BoundingBox.h"

//base class
class Shape {
    public:
        virtual bool intersect( const Ray& r, const float tmin, float &tmax, HitStruct& h) = 0;
        virtual const BoundingBox boundingBox() const = 0;
        virtual const point3 getCenter() const = 0;
    private:
        vec3 color;
};

#endif