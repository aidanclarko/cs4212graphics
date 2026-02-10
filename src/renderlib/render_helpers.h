#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

inline float random_float() {
    static std::uniform_real_distribution<float> dis(0.0, 1.0);
    static std::mt19937 gen;
    return dis(gen);
}

inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}

