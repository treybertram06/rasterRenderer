//
// Created by treyb on 2025-02-13.
//

#ifndef VEC3_H
#define VEC3_H


class Vec3 {
public:
    double e[3];
    Vec3(double x, double y, double z) : e{x, y, z} {}
    Vec3() : e{0, 0, 0} {}

    Vec3 operator+(Vec3& addend) const {
        return Vec3(e[0] + addend.e[0],
                    e[1] + addend.e[1],
                    e[2] + addend.e[2]);
    }

    Vec3 operator-(Vec3& subtrahend) const {
        return Vec3(e[0] - subtrahend.e[0],
                    e[1] - subtrahend.e[1],
                    e[2] - subtrahend.e[2]);
    }

    Vec3 operator*(double multiplier) const {
        return Vec3(e[0]*multiplier,
                    e[1]*multiplier,
                    e[2]*multiplier);
    }

    Vec3 operator/(double divisor) const {
        return Vec3(e[0]*(1/divisor),
                    e[1]*(1/divisor),
                    e[2]*(1/divisor));
    }


    /*
    double r = e[0];
    double g = e[1];
    double b = e[2];
    */

    double x = e[0];
    double y = e[1];
    double z = e[2];
};



#endif //VEC3_H
