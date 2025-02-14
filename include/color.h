//
// Created by treyb on 2025-02-13.
//

#ifndef COLOR_H
#define COLOR_H
#include "vec3.h"
#include "math_helper.h"

class Color : public Vec3 {
public:
    Color(double r, double g, double b) : Vec3(r, g, b) {}
    explicit Color(const Vec3& v) : Vec3(v.e[0], v.e[1], v.e[2]) {}
    Color() : Vec3() {}

    void convert_to_int() {
        r = round_to_int(r);
        g = round_to_int(g);
        b = round_to_int(b);
    }


    double r = e[0];
    double g = e[1];
    double b = e[2];
};

#endif //COLOR_H
