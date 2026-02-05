#ifndef SPHERE_H
#define SPHERE_H
#include "Shape.h"
#include "Ray.h"
#include "vec3.h"


class Sphere : public Shape {
    public:
        Sphere() : radius(1.0), center(point3(0,0,0)) {}
        Sphere(point3 center, float radius): radius(radius), center(center) {}
        Sphere(point3 center, float radius, vec3 color): radius(radius), center(center), color(color) {}
        /*
            refactoring with the tMin and tMax:
            IN BASE CLASS
            bool intersect(const Ray& r, float tmin, float &tmax, HitScruct& hit):        
        */

        const point3& c() const {return center;}
        const float& r() const {return radius;}

        bool intersect(const Ray& r, const float tmin, float &tmax) override {
            vec3 oc = center - r.origin();
            float a = dot(r.direction(), r.direction());
            float b =-2.0f * dot(r.direction(), oc);
            float c = dot(oc, oc) - radius * radius;

            float discriminant = b * b - 4 * a * c;
            if(discriminant < 0) { return false; }

            float t = (-b - sqrtf(discriminant)) / (2 * a);

            if(t < tmin || t > tmax) {
                t = (-b + sqrtf(discriminant)) / (2 * a);
                if(t < tmin || t > tmax) {
                    return false;
                }
            }
            tmax = t;
            return true;
        }

        const vec3& getColor() const override { return color; }

        const point3& getCenter() const override {return center;}

    private:
        float radius;
        point3 center;
        vec3 color;

};
#endif