//
// Created by treyb on 2025-02-14.
//

#ifndef DRAW_H
#define DRAW_H

#include "vec3.h"
#include "image.h"
#include "model.h"
#include "camera.h"
#include <cmath>
#include <algorithm>


class Renderer {
public:

    Renderer(Camera& camera) : camera(camera) {}
    Camera camera;

    static void put_pixel(int x, int y, const Color& c, const Image& image) {
        if (x < 0 || image.image_width <= x || y < 0 || image.image_height <= y) {
            return;
        } else {
            image.image[x][y] = c;
        }
    }

    static void draw_line(Vec3 P0, Vec3 P1, Color c, const Image& image) {
        if ( abs(P1.x - P0.x) > abs(P1.y - P0.y)) {
            //line is horizontalish
            if (P0.x > P1.x) { std::swap(P0, P1); }

            auto ys = interpolate(P0.x, P0.y, P1.x, P1.y);
            for (int x = P0.x; x <= P1.x; x++) {
                put_pixel(x, ys[x - P0.x], c, image);
            }
        } else {
            //line is verticalish
            if (P0.y > P1.y) { std::swap(P0, P1); }

            auto xs = interpolate(P0.y, P0.x, P1.y, P1.x);
            for (int y = P0.y; y <= P1.y; y++) {
                put_pixel(xs[y - P0.y], y, c, image);
            }
        }

    }

    void draw_wireframe_triangle(Vec3& P0, Vec3& P1, Vec3& P2, const Color& c, const Image& image, double viewport_info[]) {
        move_vertices(P0, P1, P2);
        project_vertices(P0, P1, P2, viewport_info);

        draw_line(projected_P0, projected_P1, c, image);
        draw_line(projected_P1, projected_P2, c, image);
        draw_line(projected_P2, projected_P0, c, image);
    }



    static void draw_shaded_triangle(Vec3& P0, Vec3& P1, Vec3& P2, const Color& c, const Image& image) {
        //sort the points so that  y0 <= y1 <= y2
        if (P1.y < P0.y) { std::swap(P1, P0); }
        if (P2.y < P0.y) { std::swap(P2, P0); }
        if (P2.y < P1.y) { std::swap(P2, P1); }

        auto x01 = interpolate(P0.y, P0.x, P1.y, P1.x);
        auto h01 = interpolate(P0.y, P0.z, P1.y, P1.z);

        auto x12 = interpolate(P1.y, P1.x, P2.y, P2.x);
        auto h12 = interpolate(P1.y, P1.z, P2.y, P2.z);

        auto x02 = interpolate(P0.y, P0.x, P2.y, P2.x);
        auto h02 = interpolate(P0.y, P0.z, P2.y, P2.z);

        x01.pop_back();
        std::vector<double> x012;
        x012.reserve(x01.size() + x12.size());
        x012.insert(x012.end(), x01.begin(), x01.end());
        x012.insert(x012.end(), x12.begin(), x12.end());

        h01.pop_back();
        std::vector<double> h012;
        h012.reserve(h01.size() + h12.size());
        h012.insert(h012.end(), h01.begin(), h01.end());
        h012.insert(h012.end(), h12.begin(), h12.end());

        double m = std::floor(x012.size() / 2.0);
        std::vector<double> x_left, x_right, h_left, h_right;
        if (x02[m] < x012[m]) {
            x_left = x02;
            h_left = h02;

            x_right = x012;
            h_right = h012;
        } else {
            x_left = x012;
            h_left = h012;

            x_right = x02;
            h_right = h02;
        }

        for (int y = P0.y; y <= P2.y; y++) {
            auto x_l = x_left[y - P0.y];
            auto x_r = x_right[y - P0.y];

            auto h_segment = interpolate(x_l, h_left[y - P0.y], x_r, h_right[y - P0.y]);
            for (int x = x_l; x <= x_r; x++) {
                Color shaded_color = Color(c * h_segment[x - x_l]);
                put_pixel(x, y, shaded_color, image);
            }
        }
    }

private:
    void move_vertices(Vec3& P0, Vec3& P1, Vec3& P2) {
        move_from_camera(P0);
        move_from_camera(P1);
        move_from_camera(P2);
    }
    void move_from_camera(Vec3& vertex) {
        rotate_x(vertex, camera.rotation.x);
        rotate_y(vertex, camera.rotation.y);
        rotate_z(vertex, camera.rotation.z);
        vertex = vertex + camera.pos;
    }

    Vec3 project_to_screen(const Vec3& vertex, double viewport_info[]) {

        double d = 1;
        double x = vertex.x / vertex.z * d;
        double y = vertex.y / vertex.z * d;

        double screen_x = (x + viewport_info[2] / 2) * viewport_info[0] / viewport_info[2];
        double screen_y = (1 - (y + viewport_info[3] / 2) / viewport_info[3]) * viewport_info[1];

        return Vec3(screen_x, screen_y, vertex.z);
    }

    Vec3 projected_P0, projected_P1, projected_P2;
    void project_vertices(const Vec3& P0, const Vec3& P1, const Vec3& P2, double viewport_info[]) {
        projected_P0 = project_to_screen(P0, viewport_info);
        projected_P1 = project_to_screen(P1, viewport_info);
        projected_P2 = project_to_screen(P2, viewport_info);
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



};



class Scene {
public:
    std::vector<Model> models;

    void add_model(const Model& model) {
        models.push_back(model);
    }

    void render(Renderer& renderer, Image& image, double viewport_info[]) {
        for (auto& model : models) {
            model.draw_wireframe(renderer, image, viewport_info);
        }
    }
};


#endif //DRAW_H
