//
// Created by treyb on 2025-02-19.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "vec3.h"
#include "triangle.h"

class Renderer;
class Image;

class Model {
public:
    Model(const std::vector<Vec3>& vertices, Vec3 pos) : vertices(vertices), pos(pos) {
        append_triangles(vertices);
    }
    Model(const std::vector<Vec3>& vertices, Vec3 pos, Color color) : vertices(vertices), pos(pos), color(color) {
        append_triangles(vertices);
    }

    void draw_wireframe(Renderer& renderer, Image& image) {
        for (const auto& triangle : triangles) {
            triangle.draw_wireframe(renderer, image);
        }
    }


private:
    void append_triangles(const std::vector<Vec3>& vertices) {
        triangles.push_back({vertices[0], vertices[1], vertices[2], color});
        triangles.push_back({vertices[0], vertices[2], vertices[3], color});
        triangles.push_back({vertices[4], vertices[0], vertices[3], color});
        triangles.push_back({vertices[4], vertices[3], vertices[7], color});
        triangles.push_back({vertices[5], vertices[4], vertices[7], color});
        triangles.push_back({vertices[5], vertices[7], vertices[6], color});
        triangles.push_back({vertices[1], vertices[5], vertices[6], color});
        triangles.push_back({vertices[1], vertices[6], vertices[2], color});
        triangles.push_back({vertices[4], vertices[5], vertices[1], color});
        triangles.push_back({vertices[4], vertices[1], vertices[0], color});
        triangles.push_back({vertices[2], vertices[6], vertices[7], color});
        triangles.push_back({vertices[2], vertices[7], vertices[3], color});
    }

    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles;
    Color color;
    Vec3 pos;


};



#endif //MODEL_H
