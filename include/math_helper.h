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

    if (std::round(i0) == std::round(i1)) { // Ensure we handle single-point cases correctly
        values.push_back(d0);
        return values;
    }

    int start = std::round(i0);
    int end = std::round(i1);
    double a = (d1 - d0) / (i1 - i0);
    double d = d0;

    int step = (start < end) ? 1 : -1; // Handle both increasing & decreasing cases

    for (int i = start; i != end + step; i += step) {
        values.push_back(d);
        d += a * step; // Adjust `d` based on direction
    }

    return values;
}


inline double interpolate_z(int x0, double z0, int x1, double z1) {
    if (x0 == x1) return z0;
    return z0 + (z1 - z0) * (x1 - x0);
}



#endif //MATH_HELPER_H
