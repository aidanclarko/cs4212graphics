#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "vec3.h"
#include "Ray.h"
#include "Sphere.h"
#include "test_helpers.h"

TEST_CASE("CONSTRUCTOR") {

    Sphere s(point3(0,0,0), 10.0);

    REQUIRE(s.r() == 10.0);
    REQUIRE(isVectorEqual(s.c(), vec3(0,0,0)));

    s = Sphere(point3(-12, 30, 23), 1.0);

    REQUIRE(s.r() == 1.0);
    REQUIRE(isVectorEqual(s.c(), vec3(-12, 30, 23)));
}

TEST_CASE("RAY SHOULD HIT SPHERE") {
    Sphere s;
    Ray r;
    float tmin = 1;
    float tmax = 100;
    
    SECTION("Ray hits sphere dead center") {
        s = Sphere(point3(0, 0, -10), 1.0);
        r = Ray(point3(0, 0, 0), vec3(0, 0, -1));
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == true);
    }
    
    SECTION("Ray starts inside sphere") {
        s = Sphere(point3(0, 0, 0), 5.0); 
        r = Ray(point3(0, 0, 0), vec3(0, 0, -1));
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == true);
    }
    
    SECTION("Ray hits sphere off-center") {
        s = Sphere(point3(0, 0, -10), 2.0);
        r = Ray(point3(1, 0, 0), vec3(0, 0, -1));
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == true);
    }
}

TEST_CASE("RAY SHOULD NOT HIT SPHERE") {
    Sphere s;
    Ray r;
    float tmin = 1;
    float tmax = 100;

     SECTION("Sphere behind camera, ray shoots forward") {
        s = Sphere(point3(0, 0, -10), 5.0);
        r = Ray(point3(0, 0, 0), vec3(0, 0, 10)); 
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == false);
    }

   
    
    SECTION("Ray misses sphere entirely") {
        s = Sphere(point3(0, 0, -10), 0.5);
        r = Ray(point3(0, 0, 0), vec3(1, 0, 0)); 
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == false);
    }
    
    SECTION("Ray just misses sphere") {
        s = Sphere(point3(0, 1.1, -10), 1.0); 
        r = Ray(point3(0, 0, 0), vec3(0, 0, -1));
        HitStruct h;
        REQUIRE(s.intersect(r, tmin, tmax, h) == false);
    }
}