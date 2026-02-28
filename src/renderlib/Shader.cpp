#include "Shader.h"
#include "Scene.h"
#include "Light.h"

bool Shader::computeShadow(HitStruct& h) {
    float epsilon = 1e-4f;
    for (auto light : h.scene->getLights()) {
        vec3 ldir = unit_vector(light->getPoint() - h.point);
        Ray shadow = Ray(h.point + vec3(epsilon, epsilon, epsilon), ldir);
        float distToLight = (light->getPoint() - h.point).length();
        HitStruct shadowHit;
        for (auto s : h.scene->getShapes()) {
            if (s->intersect(shadow, epsilon, distToLight, shadowHit)) {
                return true;
            }
        }
    }
    return false;
}