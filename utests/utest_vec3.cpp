#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "vec3.h"

constexpr float EPS = 1.0e-3f;

//machine epsilon considering float equality
bool isEqual(float a, float b, float eps = EPS) { return std::fabs(a - b) <= eps; }
// simple vector == 
bool isVectorEqual(vec3 v, vec3 u) {
    return isEqual(v.x(),u.x()) 
        && isEqual(v.y(), u.y()) 
        && isEqual(v.z(), u.z());
}

TEST_CASE("CONSTRUCTORS") {
    //default
    vec3 v;
    REQUIRE(isVectorEqual(v, vec3(0,0,0)));

    vec3 u(1.5, 2.5, 3.5);
    REQUIRE(isEqual(u.x(), 1.5));
    REQUIRE(isEqual(u.y(), 2.5));
    REQUIRE(isEqual(u.z(), 3.5));
}

TEST_CASE("VEC3 OPERATIONS") {
    // *(scalar, and another vec3), /, +; expressing operations with different values

    // vector addition
    vec3 v = vec3(1.12, 3.63, 4.94);
    vec3 u = vec3(2.01 ,5.23, 6.99);

    vec3 result = u + v;
    REQUIRE( isVectorEqual(result, vec3(3.13, 8.86, 11.93)) );
    result += u;
    REQUIRE( isVectorEqual(result, vec3(5.14, 14.09, 18.92)));
    
    //negative operator
    result = -result;
    REQUIRE( isVectorEqual(result, vec3(-5.14, -14.09, -18.92)) );

    //subtraction
    u = vec3(3.00, 1.32, 5.55);
    v = vec3(6.12, 25.21, 0.11);
    result = u - v;
    REQUIRE( isVectorEqual(result, vec3(-3.12, -23.89, 5.44)));

    //multiplication
    u = vec3(0.8, 0.1, 0.7);
    v = vec3(0.3, 0.5, 0.8);
    result = u * v;
    REQUIRE( isVectorEqual(result, vec3(0.24, 0.05, 0.56)) );
    result *= 3;
    REQUIRE( isVectorEqual(result, vec3(0.72, 0.15, 1.68)));

    //division
    u = vec3(.01, 0.82, 0.32);
    result = u / 10;
    REQUIRE( isVectorEqual(result, vec3(0.001, .082, 0.032)) );

    //scalar
    u = vec3(8.0, 1.0, 7.0);
    result = u * 3.0;
    REQUIRE( isVectorEqual(result, vec3(24.0, 3.0, 21.0)) );
}

TEST_CASE("DOT PRODUCT") {
    vec3 u;
    vec3 v;

    //basic test
    u = vec3(0.88, 1.0, .05);
    v = vec3(2.23, 0.05, 1.12);
    float result = dot(u, v);
    REQUIRE(isEqual(result, 2.0684));

    //dot property u dot v == v dot u
    result = dot(v, u);
    REQUIRE(isEqual(result, 2.0684));

    u = vec3(1.0, 0.0, 0.0);
    v = vec3(0.0, 1.0, 0.0);
    result = dot(u, v);
    REQUIRE(isEqual(result, 0.0));
}

TEST_CASE("VECTOR LENGTH") {
    //unit vector
    vec3 v = vec3(1.0, 0.0, 0.0);
    REQUIRE(isEqual(v.length(), 1.0));

    //length is always positive
    v = vec3(-3.0, -4.0, 0.0);
    REQUIRE(isEqual(v.length(), 5.0));
    
    v = vec3(1.0, 2.0, 2.0);
    REQUIRE(isEqual(v.length(), 3.0));
}

TEST_CASE("UNIT VECTOR") {
    vec3 v = vec3(2.0, 0.0, 1.0);
    vec3 unit = unit_vector(v);
    
    //unit vector should have length 1
    REQUIRE(isEqual(unit.length(), 1.0));
}

TEST_CASE("CROSS PRODUCT") {
    //u cross v 
    vec3 u = vec3(0.8, 0.5, 0.2);
    vec3 v = vec3(0.1, 0.6, 0.3);

    vec3 result = cross(u,v);
    REQUIRE(isVectorEqual(result, vec3(0.03, -0.22, 0.43)));

    //v cross u should be negative version of v cross u
    result = cross(v,u);
    REQUIRE(isVectorEqual(result, vec3(-0.03, 0.22, -0.43)));
}