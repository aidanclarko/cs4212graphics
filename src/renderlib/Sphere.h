#ifndef SPHERE_H
#define SPHERE_H
#include "Shape.h"
#include "Ray.h"
#include "vec3.h"


class Sphere : public Shape {
    public:

        Sphere(point3 center, float radius): radius(radius), center(center) {}

        bool intersect(Ray& r) override {
            vec3 oc = center - r.origin();
            float a = dot(r.direction(), r.direction());
            float b =-2.0f * dot(r.direction(), oc);
            float c = dot(oc, oc) - radius * radius;

            float discriminant = b * b - 4 * a * c;
            return (discriminant >= 0);
        }

    private:
        float radius;
        point3 center;

};
#endif