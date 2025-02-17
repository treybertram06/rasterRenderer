#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "include/image.h"
#include "include/vec2.h"
#include "include/vec3.h"
#include "include/color.h"
#include "include/renderer.h"

using namespace std;

Vec3 viewport_to_canvas(Vec2 p, double* vi) {
    double scale_x = vi[0] / vi[2];  // image_width / viewport_width
    double scale_y = vi[1] / vi[3];  // image_height / viewport_height
    return Vec3(
        (p.x * scale_x) + vi[0] / 2.0,  // Centering x
        (-p.y * scale_y) + vi[1] / 2.0, // Centering y (Y-axis is flipped)
        0
    );
}

Vec3 project_vertex(Vec3& v, double d, double* vi) {
    if (v.z == 0) v.z = 0.0001;
    return viewport_to_canvas({v.x * d / v.z, v.y * d / v.z}, vi);
}

int main() {

    int image_width  = 1000;
    int image_height = 1000;

    Color red = Color(255, 0, 0);
    Color green = Color(0, 255, 0);
    Color blue = Color(0, 0, 255);
    Color white = Color(255, 255, 255);

    clog << "Image allocated.\n";
    Image image = Image(image_width, image_height);

    double viewport_width   = 2;
    double viewport_height = viewport_width * image_height / image_width;
    double viewport_info[4] = {double(image_width), double(image_height), viewport_width, viewport_height};
    double distance_to_projection_plane = 1;


    Vec3 vAf = {-2, -0.5, 5};
    Vec3 vBf = {-2,  0.5, 5};
    Vec3 vCf = {-1,  0.5, 5};
    Vec3 vDf = {-1, -0.5, 5};

    // The four "back" vertices
    Vec3 vAb = {-2, -0.5, 6};
    Vec3 vBb = {-2,  0.5, 6};
    Vec3 vCb = {-1,  0.5, 6};
    Vec3 vDb = {-1, -0.5, 6};

    Renderer renderer;

    renderer.draw_line(project_vertex(vAf, distance_to_projection_plane, viewport_info), project_vertex(vBf, distance_to_projection_plane, viewport_info), blue, image);
    renderer.draw_line(project_vertex(vBf, distance_to_projection_plane, viewport_info), project_vertex(vCf, distance_to_projection_plane, viewport_info), blue, image);
    renderer.draw_line(project_vertex(vCf, distance_to_projection_plane, viewport_info), project_vertex(vDf, distance_to_projection_plane, viewport_info), blue, image);
    renderer.draw_line(project_vertex(vDf, distance_to_projection_plane, viewport_info), project_vertex(vAf, distance_to_projection_plane, viewport_info), blue, image);

    renderer.draw_line(project_vertex(vAb, distance_to_projection_plane, viewport_info), project_vertex(vBb, distance_to_projection_plane, viewport_info), red, image);
    renderer.draw_line(project_vertex(vBb, distance_to_projection_plane, viewport_info), project_vertex(vCb, distance_to_projection_plane, viewport_info), red, image);
    renderer.draw_line(project_vertex(vCb, distance_to_projection_plane, viewport_info), project_vertex(vDb, distance_to_projection_plane, viewport_info), red, image);
    renderer.draw_line(project_vertex(vDb, distance_to_projection_plane, viewport_info), project_vertex(vAb, distance_to_projection_plane, viewport_info), red, image);

    renderer.draw_line(project_vertex(vAf, distance_to_projection_plane, viewport_info), project_vertex(vAb, distance_to_projection_plane, viewport_info), green, image);
    renderer.draw_line(project_vertex(vBf, distance_to_projection_plane, viewport_info), project_vertex(vBb, distance_to_projection_plane, viewport_info), green, image);
    renderer.draw_line(project_vertex(vCf, distance_to_projection_plane, viewport_info), project_vertex(vCb, distance_to_projection_plane, viewport_info), green, image);
    renderer.draw_line(project_vertex(vDf, distance_to_projection_plane, viewport_info), project_vertex(vDb, distance_to_projection_plane, viewport_info), green, image);



    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}