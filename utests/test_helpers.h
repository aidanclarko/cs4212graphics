#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "vec3.h"
#include <cmath>

constexpr float EPS = 1.0e-3f;

// Machine epsilon considering float equality
inline bool isEqual(float a, float b, float eps = EPS) { 
    return std::fabs(a - b) <= eps; 
}

// Simple vector equality with epsilon tolerance
inline bool isVectorEqual(const vec3& v, const vec3& u, float eps = EPS) {
    return isEqual(v.x(), u.x(), eps) 
        && isEqual(v.y(), u.y(), eps) 
        && isEqual(v.z(), u.z(), eps);
}

#endif