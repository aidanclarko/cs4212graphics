#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "vec3.h"
#include "Shape.h"
#include "HitStruct.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct VertexPoint {
    glm::vec3 point;
    glm::vec3 normal;
};

class Triangle : public Shape {
    public:

        Triangle(point3 A, point3 B, point3 C): A(A), B(B), C(C), color(vec3(1,1,1)) {}
        Triangle(point3 A, point3 B, point3 C, vec3 color): A(A), B(B), C(C), color(color)  {}
        Triangle(point3 A, point3 B, point3 C, vec3 color, std::shared_ptr<Shader> s): A(A), B(B), C(C), color(color), shader(s)  {}
        Triangle(point3 A, point3 B, point3 C, std::shared_ptr<Shader> s): A(A), B(B), C(C), shader(s)  {}
        
        bool intersect(const Ray& r, const float tmin, float &tmax, HitStruct& hit) override {
            float a = A.x() - B.x();
            float b = A.y() - B.y();
            float c = A.z() - B.z();
            float d = A.x() - C.x();
            float e = A.y() - C.y();
            float f = A.z() - C.z();
            float g = r.direction().x();
            float h = r.direction().y();
            float i = r.direction().z();
            float j = A.x() - r.origin().x();
            float k = A.y() - r.origin().y();
            float l = A.z() - r.origin().z();

            float M = a * ((e* i) - (h * f)) + b * ((g * f) - (d * i)) + c * ((d * h) - (e * g));

            float t = -((f * ((a * k) - (j * b)) + e * ((j * c) - (a * l)) + d * ((b * l) - (k * c))) / M);


            if (t < tmin || t > tmax) {
                return false;
            }

            float gamma = (i * ((a * k) - (j * b)) + h * ((j * c) - (a * l)) + g * ((b * l) - (k * c))) / M;


            if(gamma < 0 || gamma > 1) {
                return false;
            }

            float beta = (j * ((e * i) - (h * f)) + k * ((g * f) - (d * i)) + l * ((d * h) - (e * g))) / M;

            if(beta < 0 || (beta > (1 - gamma))) {
                return false;
            }

            vec3 edgeOne = B - A;
            vec3 edgeTwo = C - A;
            vec3 normal = unit_vector(cross(edgeOne, edgeTwo));


            tmax = t;
            vec3 point = r.at(t);

            hit.shader = this->shader;
            hit.incomingRay = r;
            hit.t = t;
            hit.normal = normal;
            hit.point = point;
            return true;
        }

        glm::vec3 OpenGLNormal() {
            vec3 edgeOne = B - A;
            vec3 edgeTwo = C - A;
            vec3 normal = unit_vector(cross(edgeOne, edgeTwo));

            return glm::vec3(normal.x(), normal.y(), normal.z());
        }

        std::vector<VertexPoint> toVertexBufferSphere() {
            glm::vec3 normA = glm::normalize(glm::vec3(A.x(), A.y(), A.z()));
            glm::vec3 normB = glm::normalize(glm::vec3(B.x(), B.y(), B.z()));
            glm::vec3 normC = glm::normalize(glm::vec3(C.x(), C.y(), C.z()));

            VertexPoint vertA = { glm::vec3(A.x(), A.y(), A.z()), normA };
            VertexPoint vertB = { glm::vec3(B.x(), B.y(), B.z()), normB };
            VertexPoint vertC = { glm::vec3(C.x(), C.y(), C.z()), normC };

            return std::vector<VertexPoint>{ vertA, vertB, vertC };
        }

        std::vector<VertexPoint> toVertexBuffer() {
            glm::vec3 norm = OpenGLNormal();

            VertexPoint vertA = { glm::vec3(A.x(), A.y(), A.z()), norm };
            VertexPoint vertB = { glm::vec3(B.x(), B.y(), B.z()), norm };
            VertexPoint vertC = { glm::vec3(C.x(), C.y(), C.z()), norm };

            return std::vector<VertexPoint>{ vertA, vertB, vertC };
        }

        // const vec3& getColor() const override { return color; }

        const point3 getCenter() const override { 
            return (A + B + C) / 3.0f;
        }

        const BoundingBox boundingBox() const override {
            BoundingBox box;
            box.update(A);
            box.update(B);
            box.update(C);
            return box;
        }

        vec3 &getA()  { return A; }
        vec3 &getB()  { return B; }
        vec3 &getC()  { return C; }



    private:
        point3 A;
        point3 B;
        point3 C;
        vec3 color;
        std::shared_ptr<Shader> shader;
};

#endif