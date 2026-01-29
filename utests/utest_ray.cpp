#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Ray.h"

constexpr float EPS = 1.0e-3f;

//machine epsilon considering float equality
bool isEqual(float a, float b, float eps = EPS) { return std::fabs(a - b) <= eps; }
// simple vector == 
bool isVectorEqual(vec3 v, vec3 u) {
    return isEqual(v.x(),u.x()) 
        && isEqual(v.y(), u.y()) 
        && isEqual(v.z(), u.z());
}

// bool isRayEqual(ray a, ray b) {
//     return isVectorEqual(a.direction(), b.direction()) &&
//         isVectorEqual(a.origin(), b.origin());
// }


TEST_CASE("CONSTRUCTORS") {
    //default
    Ray r = ray();
    REQUIRE(isVectorEqual(r.direction(), vec3(0,0,0))
        && isVectorEqual(r.origin(), vec3(0,0,0))
    );

    r = ray( vec3(1.00, 0.98, 0.09), vec3(0.03, 0.01, 0.50) );
    REQUIRE(isVectorEqual(r.direction(), vec3(1.00, 0.98, 0.09))
        && isVectorEqual(r.origin(), vec3(0.03, 0.01, 0.50))
    );
}

TEST_CASE("Evaluation of Parametric Line ") {

}

TEST_CASE("Immutability") {

}

TEST_CASE("Numerical Robustness") {

}


