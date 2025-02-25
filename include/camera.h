//
// Created by treyb on 2025-02-24.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"

class Camera {
public:
    Camera(Vec3 pos, Vec3 rotation) : pos(pos * -1), rotation(rotation * -1) {}


    Vec3 pos, rotation;

};

#endif //CAMERA_H
