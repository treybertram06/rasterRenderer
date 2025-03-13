#include "include/triangle.h"
#include "include/renderer.h"

void Triangle::draw_wireframe(Renderer& renderer, Image& image, double viewport_info[]) {
    renderer.draw_wireframe_triangle(P0, P1, P2, color, image, viewport_info);
}

void Triangle::draw_filled(Renderer& renderer, Material& material, Light& light, Image& image, double viewport_info[], std::vector<std::vector<double>>& depth_buffer) {
    renderer.draw_filled_triangle_with_phong(P0, P1, P2, material, light, renderer.camera, image, viewport_info, depth_buffer);
    //renderer.draw_filled_triangle_with_depth(P0, P1, P2, color, image, viewport_info, depth_buffer);
    //renderer.draw_filled_triangle(P0, P1, P2, color, image, viewport_info);
}

void Triangle::draw_shaded(Renderer& renderer, Image& image) {
    renderer.draw_shaded_triangle(P0, P1, P2, color, image);
}

