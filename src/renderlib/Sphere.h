#ifndef SPHERE_H
#define SPHERE_H
#include <memory>
#include "Shape.h"
#include "Shader.h"
#include "Ray.h"
#include "vec3.h"
#include "HitStruct.h"


class Sphere : public Shape {
    public:
        Sphere() : radius(1.0), center(point3(0,0,0)) {}
        Sphere(point3 center, float radius): radius(radius), center(center) {}
        Sphere(point3 center, float radius, vec3 color): radius(radius), center(center), color(color) {}
        Sphere(point3 center, float radius, vec3 color, std::shared_ptr<Shader> sh): radius(radius), center(center), color(color), shader(sh) {}
        Sphere(point3 center, float radius, std::shared_ptr<Shader> sh): radius(radius), center(center), shader(sh) {}

        const point3& c() const {return center;}
        const float& r() const {return radius;}

        bool intersect(const Ray& r, const float tmin, float &tmax, HitStruct& h) override {
            vec3 oc = center - r.origin();
            float a = dot(r.direction(), r.direction());
            float b = -2.0f * dot(r.direction(), oc);
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
            vec3 point = r.at(t);
            vec3 normal = unit_vector(point - center);

            h.shader = this->shader;
            h.incomingRay = r;
            h.t = t;
            h.normal = normal;
            h.point = point;


            return true;
        }

        // const vec3& getColor() const override { return color; }

        const point3 getCenter() const override { return center; }

        const BoundingBox boundingBox() const override {
            BoundingBox box;
            vec3 llb = center - vec3(radius, radius, radius);
            vec3 urf = center + vec3(radius, radius, radius);

            box = BoundingBox(llb, urf);

            return box;
        }

    private:
        float radius;
        point3 center;
        vec3 color;
        std::shared_ptr<Shader> shader;
};
#endif