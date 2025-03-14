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

#include "light.h"
#include "material.h"


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

            auto x01 = interpolate(projected_P0.y, projected_P0.x, projected_P1.y, projected_P1.x);
            auto x12 = interpolate(projected_P1.y, projected_P1.x, projected_P2.y, projected_P2.x);
            auto x02 = interpolate(projected_P0.y, projected_P0.x, projected_P2.y, projected_P2.x);

            x01.pop_back();
            std::vector<double> x012;
            x012.reserve(x01.size() + x12.size());
            x012.insert(x012.end(), x01.begin(), x01.end());
            x012.insert(x012.end(), x12.begin(), x12.end());

            auto m = floor(x012.size() / 2);
            std::vector<double> x_left, x_right;

            if (x02[m] < x012[m]) {
                x_left = x02;
                x_right = x012;
            } else {
                x_left = x012;
                x_right = x02;
            }

            for (int y = projected_P0.y; y <= projected_P2.y; y++) {
                int left_x = x_left[y - static_cast<int>(projected_P0.y)];
                int right_x = x_right[y - static_cast<int>(projected_P0.y)];

                for (int x = left_x; x <= right_x; x++) {
                    put_pixel(x, y, c, image);
                }
            }
        }
    }

    void draw_filled_triangle_with_phong(Vec3 &P0, Vec3 &P1, Vec3 &P2, Material &material, Light &light, Camera &camera, Image &image, double viewport_info[], std::vector<std::vector<double>> &depth_buffer) {
        move_vertices_from_camera(P0, P1, P2);
        std::vector<Vec3> triangle_moved = {moved_P0, moved_P1, moved_P2};

        project_vertices(moved_P0, moved_P1, moved_P2, viewport_info);
        std::vector<Vec3> triangle_projected = {projected_P0, projected_P1, projected_P2};

        if (is_in_view(triangle_projected, viewport_info) && is_in_front(triangle_moved)) {

            //move lights
            //for (auto& light : lights) {
            Vec3 light_pos_view = move_from_camera(light.pos);
            //}
            // 1. Sort the vertices by their Y-coordinates (ascending order)
            if (projected_P1.y < projected_P0.y) std::swap(projected_P0, projected_P1);
            if (projected_P2.y < projected_P0.y) std::swap(projected_P0, projected_P2);
            if (projected_P2.y < projected_P1.y) std::swap(projected_P1, projected_P2);

            // Extract vertex coordinates
            int x0 = round_to_int(projected_P0.x), y0 = round_to_int(projected_P0.y), z0 = projected_P0.z;
            int x1 = round_to_int(projected_P1.x), y1 = round_to_int(projected_P1.y), z1 = projected_P1.z;
            int x2 = round_to_int(projected_P2.x), y2 = round_to_int(projected_P2.y), z2 = projected_P2.z;

            // 2. Split the triangle into a top and bottom part at Y1
            // Interpolate along the edges into two parts: top and bottom
            std::vector<double> x01 = interpolate(y0, x0, y1, x1); // Edge from P0 to P1
            std::vector<double> z01 = interpolate(y0, z0, y1, z1);

            std::vector<double> x02 = interpolate(y0, x0, y2, x2); // Edge from P0 to P2
            std::vector<double> z02 = interpolate(y0, z0, y2, z2);

            std::vector<double> x12 = interpolate(y1, x1, y2, x2); // Edge from P1 to P2
            std::vector<double> z12 = interpolate(y1, z1, y2, z2);

            // Remove the duplicate point at y1 when combining edges
            std::vector<double> x_left, z_left, x_right, z_right;
            if (x02.size() < x01.size() + x12.size()) {
                x_left  = x01;
                x_left.insert(x_left.end(), x12.begin() + 1, x12.end());
                z_left  = z01;
                z_left.insert(z_left.end(), z12.begin() + 1, z12.end());
                x_right = x02;
                z_right = z02;
            } else {
                x_left  = x02;
                z_left  = z02;
                x_right = x01;
                x_right.insert(x_right.end(), x12.begin() + 1, x12.end());
                z_right = z01;
                z_right.insert(z_right.end(), z12.begin() + 1, z12.end());
            }

            Vec3 triangle_normal = compute_normal(moved_P0, moved_P1, moved_P2);

            Color ambient_color = material.ambient;

            Vec3 light_dir_P0 = (light_pos_view - moved_P0).normalize();
            Vec3 light_dir_P1 = (light_pos_view - moved_P1).normalize();
            Vec3 light_dir_P2 = (light_pos_view - moved_P2).normalize();

            //Vec3 view_dir_P0 = ()
            double diffuse_strenth = std::max(0.0, light_dir_P0.dot(triangle_normal));
            auto diffuse = Color(light.intensity * diffuse_strenth);

            //needs per pixel light direction
            /*
            Vec3 reflect_source = reflect(light_dir_P0, triangle_normal);
            double specular_strength = std::max(0.0, reflect_source.dot(camera.pos));
            specular_strength = std::max(specular_strength, 1.0);
            specular_strength = pow(specular_strength, material.shininess / 256);
            auto specular = Color(light.intensity * specular_strength);
            */

            //std::clog << "Specular: " << specular.r << " " << specular.g << " " << specular.b << " " << std::endl;
            //std::clog << "Diffuse: " << diffuse.r << " " << diffuse.g << " " << diffuse.b << " " << std::endl;
            //std::clog << "Ambient: " << ambient_color.r << " " << ambient_color.g << " " << ambient_color.b << " " << std::endl;


            // 3. Render the triangle scanline by scanline
            for (int y = y0; y <= y2; y++) {
                if (y < viewport_info[1] && y >= 0) { // Bounds check on Y
                    int segment_idx = y - y0; // Y-offset into interpolated arrays
                    int xl = round_to_int(x_left[segment_idx]);
                    int xr = round_to_int(x_right[segment_idx]);

                    if (xl > xr) std::swap(xl, xr); // Ensure x_left <= x_right

                    // Compute incremental perspective-correct depth interpolation
                    double z_l_inv = 1.0 / std::max(z_left[segment_idx], 1e-6);
                    double z_r_inv = 1.0 / std::max(z_right[segment_idx], 1e-6);
                    double dz_inv = (xr != xl) ? (z_r_inv - z_l_inv) / (xr - xl) : 0;
                    double z_inv = z_l_inv;

                    for (int x = xl; x <= xr; x++, z_inv += dz_inv) {
                        if (x >= 0 && x < viewport_info[0]) { // Bounds check on X
                            double z = 1.0 / z_inv; // Recover actual depth
                            z = std::max(z, 1e-6);

                            //Vec3 light_dir_interpolated = interpolate_barycentric(x, y, moved_P0, moved_P1, moved_P2, light_dir_P0, light_dir_P1, light_dir_P2);
                            //light_dir_interpolated.normalize();

                            Vec3 frag_pos = interpolate_barycentric(x, y, projected_P0, projected_P1, projected_P2, moved_P0, moved_P1, moved_P2);
                            Vec3 light_dir = (light_pos_view - frag_pos).normalize();
                            Vec3 view_dir = (camera.pos - frag_pos).normalize();
                            Vec3 halfway_dir = (light_dir + view_dir).normalize();

                            Vec3 reflect_source = reflect(light_dir, triangle_normal);
                            reflect_source.normalize();

                            //dot of normal and halfwaydir
                            double specular_strength = std::max(0.0, halfway_dir.dot(triangle_normal.normalize()));
                            specular_strength = pow(specular_strength, material.shininess);

                            auto specular = Color(light.intensity * specular_strength);


                            auto lighting = Color(ambient_color * 1.0 + diffuse * 1.0 + specular * 0.0);
                            auto color = Color(material.color * lighting);

                            Color debug_specular =  Color(specular_strength, specular_strength, specular_strength);
                            debug_specular = Color(debug_specular + ambient_color);

                            /*
                            if (x % 100 == 0 && y % 100 == 0) {
                                std::clog << "Pixel (" << x << ", " << y << ") - Specular: "
                                          << specular.r << ", " << specular.g << ", " << specular.b << std::endl;
                            }
                            */

                            // Compare depth and update the depth buffer if closer
                            if (z <= depth_buffer[x][y]) {
                                put_pixel(x, y, color, image);
                                depth_buffer[x][y] = z;    // Update depth buffer
                            }
                        }
                    }
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

            // 1. Sort the vertices by their Y-coordinates (ascending order)
            if (projected_P1.y < projected_P0.y) std::swap(projected_P0, projected_P1);
            if (projected_P2.y < projected_P0.y) std::swap(projected_P0, projected_P2);
            if (projected_P2.y < projected_P1.y) std::swap(projected_P1, projected_P2);

            // Extract vertex coordinates
            int x0 = round_to_int(projected_P0.x), y0 = round_to_int(projected_P0.y), z0 = projected_P0.z;
            int x1 = round_to_int(projected_P1.x), y1 = round_to_int(projected_P1.y), z1 = projected_P1.z;
            int x2 = round_to_int(projected_P2.x), y2 = round_to_int(projected_P2.y), z2 = projected_P2.z;

            // 2. Split the triangle into a top and bottom part at Y1
            // Interpolate along the edges into two parts: top and bottom
            std::vector<double> x01 = interpolate(y0, x0, y1, x1); // Edge from P0 to P1
            std::vector<double> z01 = interpolate(y0, z0, y1, z1);

            std::vector<double> x02 = interpolate(y0, x0, y2, x2); // Edge from P0 to P2
            std::vector<double> z02 = interpolate(y0, z0, y2, z2);

            std::vector<double> x12 = interpolate(y1, x1, y2, x2); // Edge from P1 to P2
            std::vector<double> z12 = interpolate(y1, z1, y2, z2);

            // Remove the duplicate point at y1 when combining edges
            std::vector<double> x_left, z_left, x_right, z_right;
            if (x02.size() < x01.size() + x12.size()) {
                x_left  = x01;
                x_left.insert(x_left.end(), x12.begin() + 1, x12.end());
                z_left  = z01;
                z_left.insert(z_left.end(), z12.begin() + 1, z12.end());
                x_right = x02;
                z_right = z02;
            } else {
                x_left  = x02;
                z_left  = z02;
                x_right = x01;
                x_right.insert(x_right.end(), x12.begin() + 1, x12.end());
                z_right = z01;
                z_right.insert(z_right.end(), z12.begin() + 1, z12.end());
            }

            // 3. Render the triangle scanline by scanline
            for (int y = y0; y <= y2; y++) {
                if (y < viewport_info[1] && y >= 0) { // Bounds check on Y
                    int segment_idx = y - y0; // Y-offset into interpolated arrays
                    int xl = round_to_int(x_left[segment_idx]);
                    int xr = round_to_int(x_right[segment_idx]);

                    if (xl > xr) std::swap(xl, xr); // Ensure x_left <= x_right

                    // Compute incremental perspective-correct depth interpolation
                    double z_l_inv = 1.0 / std::max(z_left[segment_idx], 1e-6);
                    double z_r_inv = 1.0 / std::max(z_right[segment_idx], 1e-6);
                    double dz_inv = (xr != xl) ? (z_r_inv - z_l_inv) / (xr - xl) : 0;
                    double z_inv = z_l_inv;

                    for (int x = xl; x <= xr; x++, z_inv += dz_inv) {
                        if (x >= 0 && x < viewport_info[0]) { // Bounds check on X
                            double z = 1.0 / z_inv; // Recover actual depth
                            z = std::max(z, 1e-6);

                            // Compare depth and update the depth buffer if closer
                            if (z <= depth_buffer[x][y]) {
                                put_pixel(x, y, c, image); // Draw pixel
                                depth_buffer[x][y] = z;    // Update depth buffer
                            }
                        }
                    }
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

    Vec3 compute_normal(Vec3& P0, Vec3& P1, Vec3& P2) {
        Vec3 edge1 = P1 - P0;
        Vec3 edge2 = P2 - P0;
        return edge2.cross(edge1).normalize();
    }

    Vec3 reflect(const Vec3& vector, Vec3& normal) {
        Vec3 normalized_normal = normal.normalize();

        double dot_product = vector.dot(normalized_normal);
        Vec3 reflection =  normalized_normal * vector - 2 * dot_product;

        return reflection;
    }

    Vec3 interpolate_barycentric(int x, int y, Vec3 P0, Vec3 P1, Vec3 P2, Vec3 V0, Vec3 V1, Vec3 V2) {
        Vec3 v0 = P1 - P0, v1 = P2 - P0, v2 = Vec3(x, y, 0) - P0;
        double d00 = v0.dot(v0);
        double d01 = v0.dot(v1);
        double d11 = v1.dot(v1);
        double d20 = v2.dot(v0);
        double d21 = v2.dot(v1);
        double denom = d00 * d11 - d01 * d01;

        double v = (d11 * d20 - d01 * d21) / denom;
        double w = (d00 * d21 - d01 * d20) / denom;
        double u = 1.0 - v - w;

        return V0 * u + V1 * v + V2 * w;
    }





};



class Scene {
public:
    std::vector<Model> models;

    void add_model(const Model& model) {
        models.push_back(model);
    }

    void render(Renderer& renderer, Material& material, Light& light, Image& image, double viewport_info[], std::vector<std::vector<double>>& depth_buffer) {
        for (auto& model : models) {
            auto culled = cull_back_faces(model, renderer.camera);
            culled.draw_filled(renderer, material, light, image, viewport_info, depth_buffer);
            //model.draw_wireframe(renderer, image, viewport_info);
        }
    }

private:
    Model cull_back_faces(Model model, Camera& camera) {
        auto it = model.triangles.begin();
        while (it != model.triangles.end()) {
            if (is_back_facing(*it, camera)) {
                // Remove the back-facing triangle from the model
                it = model.triangles.erase(it);
            } else {
                ++it;
            }
        }
        return model;
    }

    bool is_back_facing(Triangle& triangle, Camera& camera) {
        Vec3 edge1 = triangle.P1 - triangle.P0;
        Vec3 edge2 = triangle.P2 - triangle.P0;
        Vec3 normal = edge1.cross(edge2).normalize();

        Vec3 camera_to_triangle = triangle.P0 - camera.pos;

        return normal.dot(camera_to_triangle) < 0;
    }
};


#endif //DRAW_H
