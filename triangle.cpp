#include "include/triangle.h"
#include "include/renderer.h"

void Triangle::draw_wireframe(Renderer& renderer, Image& image) const {
    renderer.draw_wireframe_triangle(P0, P1, P2, color, image);
}

void Triangle::draw_shaded(Renderer& renderer, Image& image) {
    renderer.draw_shaded_triangle(P0, P1, P2, color, image);
}
