//
// Created by treyb on 2025-02-19.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vec3.h"
#include "triangle.h"
#include <iostream>
#include <cmath>


class Renderer;
class Image;

class Model {
public:
    Model(const std::vector<Vec3>& vertices, Vec3 pos) : vertices(vertices), pos(pos) {
        //order is very important
        translate_vertices();
        append_triangles();
    }
    Model(const std::vector<Vec3>& vertices, Vec3 pos, Color color) : vertices(vertices), pos(pos), color(color) {
        //order is very important
        translate_vertices();
        append_triangles();
    }
    Model(const std::vector<Vec3>& vertices, Vec3 pos, Vec3 rotation, double scale, Color color) : vertices(vertices), pos(pos), rotation(rotation), scale(scale), color(color) {
        //order is very important
        rotate_vertices();
        scale_vertices();
        translate_vertices();
        append_triangles();
    }


    void draw_wireframe(Renderer& renderer, Image& image, double viewport_info[]) {
        for (auto& triangle : triangles) {
            triangle.draw_wireframe(renderer, image, viewport_info);
        }
    }

    void draw_filled(Renderer& renderer, Material& material, Light& light, Image& image, double viewport_info[], std::vector<std::vector<double>>& depth_buffer) {
        for (auto& triangle : triangles) {
            triangle.draw_filled(renderer, material, light, image, viewport_info, depth_buffer);
        }
    }

    Color color;
    Vec3 pos;
    Vec3 rotation;
    double scale;
    std::vector<Triangle> triangles;


private:

    // CURRENTLY USING RANDOM COLORS
    void append_triangles() {
        triangles.push_back({vertices[0], vertices[1], vertices[2], get_random_color()});
        triangles.push_back({vertices[0], vertices[2], vertices[3], get_random_color()});
        triangles.push_back({vertices[4], vertices[0], vertices[3], get_random_color()});
        triangles.push_back({vertices[4], vertices[3], vertices[7], get_random_color()});
        triangles.push_back({vertices[5], vertices[4], vertices[7], get_random_color()});
        triangles.push_back({vertices[5], vertices[7], vertices[6], get_random_color()});
        triangles.push_back({vertices[1], vertices[5], vertices[6], get_random_color()});
        triangles.push_back({vertices[1], vertices[6], vertices[2], get_random_color()});
        triangles.push_back({vertices[4], vertices[5], vertices[1], get_random_color()});
        triangles.push_back({vertices[4], vertices[1], vertices[0], get_random_color()});
        triangles.push_back({vertices[2], vertices[6], vertices[7], get_random_color()});
        triangles.push_back({vertices[2], vertices[7], vertices[3], get_random_color()});
    }

    /*
    void append_triangles() {
        triangles.push_back({vertices[2], vertices[0], vertices[1], get_random_color()});
        triangles.push_back({vertices[2], vertices[1], vertices[3], get_random_color()});

        // Side Faces (Make sure they are CCW when viewed from outside)
        triangles.push_back({vertices[1], vertices[0], vertices[4], get_random_color()}); // Front
        triangles.push_back({vertices[3], vertices[1], vertices[4], get_random_color()}); // Right
        triangles.push_back({vertices[2], vertices[3], vertices[4], get_random_color()}); // Back
        triangles.push_back({vertices[0], vertices[2], vertices[4], get_random_color()}); // Left

    }
    */

    Color get_random_color() {
        return {double(rand() % 256), double(rand() % 256), double(rand() % 256)};
    }

    void translate_vertices() {
        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = pos - vertices[i];
            //std::clog << "Translated: " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
        }
    }

    void scale_vertices() {
        for (auto& v : vertices) {
            v = v * scale;
        }
    }

    //reminder to future me, if you want to represent the same thing with two different names, just make one reference the other...
    void rotate_vertices() {
        //std::clog << "Rotating vertices..." << std::endl;
        for (int i = 0; i < vertices.size(); i++) {
            //std::clog << "Before: (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
            rotate_x(vertices[i], rotation.x);
            rotate_y(vertices[i], rotation.y);
            rotate_z(vertices[i], rotation.z);
            //std::clog << "After: (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
        }
    }

    void rotate_x(Vec3& v, double angle) {
        double rad = to_radians(angle);
        double y = v.y * cos(rad) - v.z * sin(rad);
        double z = v.y * sin(rad) + v.z * cos(rad);
        v.y = y;
        v.z = z;
    }

    void rotate_y(Vec3& v, double angle) {
        double rad = to_radians(angle);
        double x = v.x * cos(rad) + v.z * sin(rad);
        double z = -v.x * sin(rad) + v.z * cos(rad);
        v.x = x;
        v.z = z;
    }

    void rotate_z(Vec3& v, double angle) {
        double rad = to_radians(angle);
        double x = v.x * cos(rad) - v.y * sin(rad);
        double y = v.x * sin(rad) + v.y * cos(rad);
        v.x = x;
        v.y = y;
    }

    std::vector<Vec3> vertices;




};



#endif //MODEL_H
