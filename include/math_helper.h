//
// Created by treyb on 2025-02-13.
//

#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <vector>
#include <cmath>
#include "vec3.h"

inline double to_radians(double degrees) { return degrees * M_PI / 180.0; }

inline int round_to_int(double n) {
    n = n + 0.5 - (n<0);
    return static_cast<int>(n);
}

inline std::vector<double> interpolate(double i0, double d0, double i1, double d1) {
    std::vector<double> values;
    if (i0 == i1) {
        values.push_back(d0);
        return values;
    }

    double a = (d1 - d0) / (i1 - i0);
    double d = d0;

    for (int i = i0; i <= i1; i++) {
        values.push_back(d);
        d = d+a;
    }
    return values;

}

#endif //MATH_HELPER_H
