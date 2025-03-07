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
    void set_camera(Camera& new_camera) { camera = new_camera; }
    Camera camera;

    static void put_pixel(int x, int y, const Color& c, const Image& image) {
        if (x < 0 || image.image_width <= x || y < 0 || image.image_height <= y) {
            return;
        }
        image.image[x][y] = c;
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
        move_vertices_from_camera(P0, P1, P2);
        std::vector<Vec3> triangle_moved = {moved_P0, moved_P1, moved_P2};

        project_vertices(moved_P0, moved_P1, moved_P2, viewport_info);
        std::vector<Vec3> triangle_projected = {projected_P0, projected_P1, projected_P2};

        if (is_in_view(triangle_projected, viewport_info) && is_in_front(triangle_moved)) {

            draw_line(projected_P0, projected_P1, c, image);
            draw_line(projected_P1, projected_P2, c, image);
            draw_line(projected_P2, projected_P0, c, image);
        }
    }

    void draw_filled_triangle(Vec3& P0, Vec3& P1, Vec3& P2, const Color& c, Image& image, double viewport_info[]) {
    move_vertices_from_camera(P0, P1, P2);
    std::vector<Vec3> triangle_moved = {moved_P0, moved_P1, moved_P2};

    project_vertices(moved_P0, moved_P1, moved_P2, viewport_info);
    std::vector<Vec3> triangle_projected = {projected_P0, projected_P1, projected_P2};

    if (is_in_view(triangle_projected, viewport_info) && is_in_front(triangle_moved)) {

        if (projected_P1.y < projected_P0.y) std::swap(projected_P1, projected_P0);
        if (projected_P2.y < projected_P0.y) std::swap(projected_P2, projected_P0);
        if (projected_P2.y < projected_P1.y) std::swap(projected_P2, projected_P1);

        std::vector<double> x01 = interpolate(projected_P0.y, projected_P0.x, projected_P1.y, projected_P1.x);
        std::vector<double> x12 = interpolate(projected_P1.y, projected_P1.x, projected_P2.y, projected_P2.x);
        std::vector<double> x02 = interpolate(projected_P0.y, projected_P0.x, projected_P2.y, projected_P2.x);

        std::vector<double> x012 = x01;
        x012.pop_back();
        x012.insert(x012.end(), x12.begin(), x12.end());

        size_t m = x012.size() / 2;
        std::vector<double> x_left, x_right;

        if (x02[m] < x012[m]) {
            x_left = x02;
            x_right = x012;
        } else {
            x_left = x012;
            x_right = x02;
        }

        for (int y = static_cast<int>(projected_P0.y); y <= static_cast<int>(projected_P2.y); y++) {
            int left_x = static_cast<int>(x_left[y - static_cast<int>(projected_P0.y)]);
            int right_x = static_cast<int>(x_right[y - static_cast<int>(projected_P0.y)]);

            for (int x = left_x; x <= right_x; x++) {
                put_pixel(x, y, c, image);
            }
        }
    }
}

    void draw_filled_triangle_with_depth(Vec3& P0, Vec3& P1, Vec3& P2, const Color& c, Image& image, double viewport_info[], std::vector<std::vector<double>>& depth_buffer) {
    move_vertices_from_camera(P0, P1, P2);
    std::vector<Vec3> triangle_moved = {moved_P0, moved_P1, moved_P2};

    project_vertices(moved_P0, moved_P1, moved_P2, viewport_info);
    std::vector<Vec3> triangle_projected = {projected_P0, projected_P1, projected_P2};

    if (is_in_view(triangle_projected, viewport_info) && is_in_front(triangle_moved)) {

        if (projected_P1.y < projected_P0.y) std::swap(projected_P1, projected_P0);
        if (projected_P2.y < projected_P0.y) std::swap(projected_P2, projected_P0);
        if (projected_P2.y < projected_P1.y) std::swap(projected_P2, projected_P1);

        std::vector<double> x01 = interpolate(projected_P0.y, projected_P0.x, projected_P1.y, projected_P1.x);
        std::vector<double> x12 = interpolate(projected_P1.y, projected_P1.x, projected_P2.y, projected_P2.x);
        std::vector<double> x02 = interpolate(projected_P0.y, projected_P0.x, projected_P2.y, projected_P2.x);

        std::vector<double> z01 = interpolate(projected_P0.y, projected_P0.z, projected_P1.y, projected_P1.z);
        std::vector<double> z12 = interpolate(projected_P1.y, projected_P1.z, projected_P2.y, projected_P2.z);
        std::vector<double> z02 = interpolate(projected_P0.y, projected_P0.z, projected_P2.y, projected_P2.z);

        std::vector<double> x012 = x01;
        x012.pop_back();
        x012.insert(x012.end(), x12.begin(), x12.end());

        std::vector<double> z012 = z01;
        z012.pop_back();
        z012.insert(z012.end(), z12.begin(), z12.end());

        size_t m = x012.size() / 2;
        std::vector<double> x_left, x_right, z_left, z_right;

        if (x02[m] < x012[m]) {
            x_left = x02;
            x_right = x012;
            z_left = z02;
            z_right = z012;
        } else {
            x_left = x012;
            x_right = x02;
            z_left = z012;
            z_right = z02;
        }

        for (int y = static_cast<int>(projected_P0.y); y <= static_cast<int>(projected_P2.y); y++) {
            int left_x = static_cast<int>(x_left[y - static_cast<int>(projected_P0.y)]);
            int right_x = static_cast<int>(x_right[y - static_cast<int>(projected_P0.y)]);

            double z_left_value = z_left[y - static_cast<int>(projected_P0.y)];
            double z_right_value = z_right[y - static_cast<int>(projected_P0.y)];

            double z_segment = interpolate_z(left_x, z_left_value, right_x, z_right_value);

            for (int x = left_x; x <= right_x; x++) {
                if (z_segment < depth_buffer[x][y]) {
                    put_pixel(x, y, c, image);
                    depth_buffer[x][y] = z_segment;
                }
                z_segment += (z_right_value - z_left_value) / (right_x - left_x);
            }
        }
    }
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
    Vec3 moved_P0, moved_P1, moved_P2;
    void move_vertices_from_camera(Vec3& P0, Vec3& P1, Vec3& P2) {
        moved_P0 = move_from_camera(P0);
        moved_P1 = move_from_camera(P1);
        moved_P2 = move_from_camera(P2);
    }
    Vec3 move_from_camera(Vec3& vertex) {
        Vec3 moved_vertex = vertex - camera.pos;

        rotate_x(moved_vertex, camera.rotation.x);
        rotate_y(moved_vertex, camera.rotation.y);
        rotate_z(moved_vertex, camera.rotation.z);

        return moved_vertex;
    }


    Vec3 project_to_screen(const Vec3& vertex, double viewport_info[]) {

        double image_width = viewport_info[0];
        double image_height = viewport_info[1];
        double viewport_width = viewport_info[2];
        double viewport_height = viewport_info[3];

        double d = 1;
        double x = vertex.x / vertex.z * d;
        double y = vertex.y / vertex.z * d;

        double screen_x = (x + viewport_width / 2) * image_width / viewport_width;
        double screen_y = (1 - (y + viewport_height / 2) / viewport_height) * image_height;

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

    bool is_in_view(const std::vector<Vec3>& triangle, double viewport_info[]) {
        int count = 0;
        for (int i = 0; i < 3; i++) {
            if ( triangle[i].x < 0 || triangle[i].x > viewport_info[0]) {
                //std::clog << "vertex is off screen.\n";
                count++;
            }
            if ( triangle[i].y < 0 || triangle[i].y > viewport_info[1]) {
                //std::clog << "vertex is off screen.\n";
                count++;
            }
        }

        return !(count == 3);
    }

    bool is_in_front(const std::vector<Vec3>& triangle) {
        for (int i = 0; i < 3; i++) {
            if ( triangle[i].z < 1) {
                //std::clog << "behind camera\n";
                return false;
            }
        }
        return true;
    }



};



class Scene {
public:
    std::vector<Model> models;

    void add_model(const Model& model) {
        models.push_back(model);
    }

    void render(Renderer& renderer, Image& image, double viewport_info[], std::vector<std::vector<double>>& depth_buffer) {
        for (auto& model : models) {
            model.draw_filled(renderer, image, viewport_info, depth_buffer);
        }
    }
};


#endif //DRAW_H
