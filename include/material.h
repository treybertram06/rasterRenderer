//
// Created by treyb on 2025-03-13.
//

#ifndef MATERIAL_H
#define MATERIAL_H
#include "color.h"

class Material {
public:
    Color ambient, diffuse, specular, color;
    double shininess;
};

#endif //MATERIAL_H
