//
// Created by treyb on 2025-02-24.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include <vector>

class Camera {
public:
    Camera(Vec3 pos, Vec3 rotation, double viewport_width, double viewport_height)
    : pos(pos), rotation(rotation), viewport_width(viewport_width), viewport_height(viewport_height) {
        viewport_corners.push_back(Vec3(-viewport_width / 2, viewport_height / 2, -1));     //tl
        viewport_corners.push_back(Vec3(viewport_width / 2, viewport_height / 2, -1));      //tr
        viewport_corners.push_back(Vec3(-viewport_width / 2, -viewport_height / 2, -1));    //bl
        viewport_corners.push_back(Vec3(viewport_width / 2, -viewport_height / 2, -1));     //br
    }
    Camera() {}


    std::vector<Vec3> get_viewport_rays() {
        std::vector<Vec3> rays;
        for (auto& corner : viewport_corners) {
            // Rotate the corner point (relative to the camera's orientation)
            Vec3 rotated = rotatePoint(corner);

            // Calculate the direction vector from the camera to the rotated corner
            Vec3 direction = (rotated - pos).normalize(); // Use the camera position to find the direction
            rays.push_back(direction);
        }
        return rays;
    }


    Vec3 pos, rotation;
    double viewport_width, viewport_height;
    std::vector<Vec3> viewport_corners;


private:
    Vec3 rotatePoint(const Vec3& point) const {
        // Get rotation values
        double yaw = rotation.y;   // Yaw (around Y-axis)
        double pitch = rotation.x; // Pitch (around X-axis)
        double roll = rotation.z;  // Roll (around Z-axis)

        // Yaw (Y-axis rotation)
        double cosy = cos(yaw), siny = sin(yaw);
        Vec3 rotated_yaw(
            point.x * cosy - point.z * siny,
            point.y,
            point.x * siny + point.z * cosy
        );

        // Pitch (X-axis rotation)
        double cosp = cos(pitch), sinp = sin(pitch);
        Vec3 rotated_pitch(
            rotated_yaw.x,
            rotated_yaw.y * cosp - rotated_yaw.z * sinp,
            rotated_yaw.y * sinp + rotated_yaw.z * cosp
        );

        // Roll (Z-axis rotation)
        double cosr = cos(roll), sinr = sin(roll);
        Vec3 rotated_roll(
            rotated_pitch.x * cosr - rotated_pitch.y * sinr,
            rotated_pitch.x * sinr + rotated_pitch.y * cosr,
            rotated_pitch.z
        );

        return rotated_roll;
    }




};

#endif //CAMERA_H
