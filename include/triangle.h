#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"
#include "color.h"
#include "image.h"

// Forward declaration instead of including renderer.h
class Renderer;

class Triangle {
public:
    Vec3 P0, P1, P2;
    Color color;

    Triangle() = default;
    Triangle(Vec3 P0, Vec3 P1, Vec3 P2) : P0(P0), P1(P1), P2(P2), color(Color()) {}
    Triangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color) : P0(P0), P1(P1), P2(P2), color(color) {}

    void draw_wireframe(Renderer& renderer, Image& image) const;
    void draw_shaded(Renderer& renderer, Image& image);

    Vec3 project_to_screen(const Vec3& vertex, double viewport_info[]) {
        double d = 1;
        double x = vertex.x / vertex.z * d;
        double y = vertex.y / vertex.z * d;

        double screen_x = (x + viewport_info[2] / 2) * viewport_info[0] / viewport_info[2];
        double screen_y = (1 - (y + viewport_info[3] / 2) / viewport_info[3]) * viewport_info[1];

        return Vec3(screen_x, screen_y, vertex.z);
    }

    void project_vertices(double viewport_info[]) {
        projected_P0 = project_to_screen(P0, viewport_info);
        projected_P1 = project_to_screen(P1, viewport_info);
        projected_P2 = project_to_screen(P2, viewport_info);
    }

private:
    Vec3 projected_P0, projected_P1, projected_P2;



};

#endif // TRIANGLE_H
