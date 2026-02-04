#ifndef SPHERE_H
#define SPHERE_H
#include "Shape.h"
#include "Ray.h"
#include "vec3.h"


class Sphere : public Shape {
    public:

        Sphere(point3 center, float radius): radius(radius), center(center) {}

        bool intersect(const Ray &r) override {
            float A = dot(r.direction(), r.direction());
            float B = dot((2.0f * r.direction()), (r.origin() - center));
            float C = dot((r.origin()- center), ((r.origin() - center))) - (radius * radius);

            float discriminant = (B * B) - 4 * (A * C);
            return discriminant >= 0;
        }

    private:
        float radius;
        point3 center;

};
#endif