#ifndef SHAPE_H
#define SHAPE_H

#include "Ray.h"

//base class
class Shape {
    public:
        virtual bool intersect(const Ray& r) = 0;
    private:
};

#endif