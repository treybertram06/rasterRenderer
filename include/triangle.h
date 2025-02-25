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

    void draw_wireframe(Renderer& renderer, Image& image, double viewport_info[]);
    void draw_shaded(Renderer& renderer, Image& image);


};

#endif // TRIANGLE_H
