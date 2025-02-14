//
// Created by treyb on 2025-02-13.
//

#ifndef COLOR_H
#define COLOR_H
#include "vec3.h"

class Color : Vec3 {
public:
    Color(double r, double g, double b) : Vec3(r, g, b) {}
    Color() : Vec3() {}

    double r = e[0];
    double g = e[1];
    double b = e[2];
};

#endif //COLOR_H
