//
// Created by Jorge on 12/18/2024.
//

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.141415926535897932385;


// Utility functions
inline double degrees_to_radians (double degrees) {
    return degrees * pi/ 180.0;
}

inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

// Common headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif //RTWEEKEND_H
