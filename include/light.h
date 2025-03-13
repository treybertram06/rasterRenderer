//
// Created by treyb on 2025-03-13.
//

#ifndef LIGHT_H
#define LIGHT_H
#include "color.h"

class Light {
public:
    Vec3 pos;
    double radius;
    Color intensity;
};

#endif //LIGHT_H
