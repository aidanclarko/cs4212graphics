#include "Shader.h"
#include "Scene.h"
#include "Light.h"

bool Shader::computeShadow(HitStruct& h, std::shared_ptr<Light> light) {
    float epsilon = 1e-3f;
    vec3 ldir = unit_vector(light->getPoint() - h.point);
    Ray shadow = Ray(h.point + h.normal * epsilon, ldir);
    float distToLight = (light->getPoint() - h.point).length();
    HitStruct shadowHit;

    if (h.scene->getBVH()->intersect(shadow, epsilon, distToLight, shadowHit)) {
        return true;
    }
    return false;
}