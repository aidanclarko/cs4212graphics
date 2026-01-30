#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Ray.h"
#include "test_helpers.h"


TEST_CASE("CONSTRUCTORS") {
    Ray r;

    SECTION("DEFAULT CONSTRUCTOR") {
        REQUIRE((isVectorEqual(r.direction(), vec3(0,0,0))
            && isVectorEqual(r.origin(), vec3(0,0,0)))
        );
    }

    SECTION("RANDOM CONSTRUCTOR SET") {
        r = Ray( vec3(1.00, 0.98, 0.09), vec3(0.03, 0.01, 0.50) );
        REQUIRE((isVectorEqual(r.origin(), vec3(1.00, 0.98, 0.09))
            && isVectorEqual(r.direction(), vec3(0.03, 0.01, 0.50)))
        );
    }
}

TEST_CASE("Evaluation of Parametric Line ") {
    Ray r;

    r.origin() = vec3(0,0,0);
    r.direction() = vec3(0,1,0);

    SECTION("AT 0 RETURN ORIGIN") {
        REQUIRE( isVectorEqual(r.at(0.0), vec3(0,0,0)) );
    }

    SECTION("AT 1 RETURN DIRECTION") {
        REQUIRE( isVectorEqual(r.at(1.0), vec3(0,1,0)) );
    }

    SECTION("POSITIVE FLOAT VALUES") {
        r = Ray(
            vec3(0.13, 0.55, 0.99),
            vec3(0.0, 0.654, 0.786)
        );

        REQUIRE( isVectorEqual(r.at(0.13), vec3(0.13, 0.63502, 1.09218)) );
    }

    SECTION("NEGATIVE FLOAT VALUES") {
        r = Ray(
            vec3(-9.0, -8.5, -2.3),
            vec3(-6.4, -3.2, -2.1)
        );

        REQUIRE( isVectorEqual(r.at(-3.5), vec3(13.4, 2.7, 5.05)) );
    }
}

TEST_CASE("Immutability") {
    Ray r = Ray(vec3(1.0, 2.0, 3.0), vec3(4.0, 5.0, 6.0));
    vec3 original_origin = r.origin();
    vec3 original_direction = r.direction();

    SECTION("VERIFY IMMUT WITH A LOT OF FUNCTION CALLS") {
        r.at(0.0);
        r.at(1.0);
        r.at(5.5);
        r.at(-2.3);
        
        REQUIRE(isVectorEqual(r.origin(), original_origin));
        REQUIRE(isVectorEqual(r.direction(), original_direction));
    }

    SECTION("COPY WITH DEFAULT CONSTRUCTOR IMMUTBILITY TEST") {
        Ray copy = r;

        REQUIRE( isVectorEqual(copy.origin(), original_origin) );
        REQUIRE( isVectorEqual(copy.direction(), original_direction) );
    }

    SECTION("COPY/OVERLOAD EXISTING RAY WITH VALUES") {
        Ray copy = Ray(vec3(0,0,0), vec3(1,1,1));
        copy = r;
        
        REQUIRE( isVectorEqual(copy.origin(), original_origin) );
        REQUIRE( isVectorEqual(copy.direction(), original_direction) );
    }
}

TEST_CASE("Numerical Robustness") {
    Ray r;

    SECTION("LARGE T VALUE") {
        r = Ray(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
        vec3 res = r.at(100000);
        REQUIRE( isVectorEqual(res, vec3(100000,100000,100000)) );
    }

    SECTION("SMALL T VALUE") {
        r = Ray(vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
        vec3 res = r.at(0.00001);
        REQUIRE( isVectorEqual(res, vec3(0.00001,0.00001,0.00001)));
    }

    SECTION("MIX OF NEGATIVE AND POSITIVE COMPONENTS") {
        r = Ray(vec3(-1.0, 2.0, 3.0), vec3(-4.0, -5.0, 6.0));
        vec3 res = r.at(-2.0);
        REQUIRE( isVectorEqual(res, vec3(7.0, 12.0, -9.0)));
    }

    SECTION("ZERO DIRECTION VECTOR SHOULD EVALUATE") {
        r = Ray(vec3(5.0, 5.0, 5.0), vec3(0.0, 0.0,  0.0));
        vec3 res = r.at(100.0);
        REQUIRE( isVectorEqual(res, vec3(5.0, 5.0, 5.0)));
    }
}


