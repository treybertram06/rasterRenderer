#include "include/triangle.h"
#include "include/renderer.h"

void Triangle::draw_wireframe(Renderer& renderer, Image& image) const {
    renderer.draw_wireframe_triangle(projected_P0, projected_P1, projected_P2, color, image);
}

void Triangle::draw_shaded(Renderer& renderer, Image& image) {
    renderer.draw_shaded_triangle(projected_P0, projected_P1, projected_P2, color, image);
}

