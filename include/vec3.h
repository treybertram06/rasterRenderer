//
// Created by treyb on 2025-02-13.
//

#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include "math_helper.h"

class Vec3 {
public:
    double x, y, z;
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3() : x(0), y(0), z(0) {}

    Vec3 operator+(const Vec3& addend) const {
        return Vec3(x + addend.x,
                    y + addend.y,
                    z + addend.z);
    }

    Vec3 operator+=(const Vec3& addend) {
        x += addend.x;
        y += addend.y;
        z += addend.z;
        return *this;
    }


    Vec3 operator-(Vec3& subtrahend) const {
        return Vec3(x - subtrahend.x,
                    y - subtrahend.y,
                    z - subtrahend.z);
    }

    Vec3 operator*(double multiplier) const {
        return Vec3(x*multiplier,
                    y*multiplier,
                    z*multiplier);
    }

    Vec3 operator*=(double multiplier) {
        x *= multiplier;
        y *= multiplier;
        z *= multiplier;
        return *this;
    }

    Vec3 operator/(double divisor) const {
        return Vec3(x*(1/divisor),
                    y*(1/divisor),
                    z*(1/divisor));
    }

    bool operator==(Vec3 comparator) {
        if (x == comparator.x &&
            y == comparator.y &&
            z == comparator.z) {
            return true;
        } else {
            return false;
        }
    }


};



#endif //VEC3_H
