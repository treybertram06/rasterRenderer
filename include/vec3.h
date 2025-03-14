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


    Vec3 operator-(const Vec3& subtrahend) const {
        return Vec3(x - subtrahend.x,
                    y - subtrahend.y,
                    z - subtrahend.z);
    }

    Vec3 operator-() {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator-(double subtrahend) const {
        return Vec3(x - subtrahend,
                    y - subtrahend,
                    z - subtrahend);
    }

    Vec3 operator-=(const Vec3& subtrahend) {
        x -= subtrahend.x;
        y -= subtrahend.y;
        z -= subtrahend.z;
        return *this;
    }

    Vec3 operator*(double multiplier) const {
        return Vec3(x*multiplier,
                    y*multiplier,
                    z*multiplier);
    }

    Vec3 operator*(Vec3 multiplier) const {
        return Vec3(x*multiplier.x,
                    y*multiplier.y,
                    z*multiplier.z);
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

    bool operator<(const Vec3& other) const {
        return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
    }

    void translate(const Vec3& tranlation) {
        x += tranlation.x;
        y += tranlation.y;
        z += tranlation.z;
    }

    Vec3 normalize() {
        double mag = magnitude();
        if (mag > 0) {
            return Vec3(x / mag, y / mag, z / mag);
        }
        return Vec3(0.0, 0.0, 0.0); // Return zero vector if magnitude is zero
    }


    double magnitude() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

     inline double dot(const Vec3& other) const {
        return (
            x * other.x +
            y * other.y +
            z * other.z);
    }

    Vec3 reflect(Vec3& normal)  {
        Vec3 normalized_normal = normal.normalize();

        double dot_product = this->dot(normalized_normal);
        Vec3 reflection = *this - (normalized_normal * (2 * dot_product));

        return reflection;
    }


};



#endif //VEC3_H
