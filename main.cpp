#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "include/image.h"
#include "include/vec2.h"
#include "include/vec3.h"
#include "include/color.h"
#include "include/renderer.h"
#include "include/model.h"

using namespace std;


int main() {

    int image_width  = 1000;
    int image_height = 1000;



    clog << "Image allocated.\n";
    Image image = Image(image_width, image_height);

    double viewport_width   = 2;
    double viewport_height = viewport_width * image_height / image_width;
    double viewport_info[4] = {double(image_width), double(image_height), viewport_width, viewport_height};
    double distance_to_projection_plane = 1;


    Triangle renderer;

    Vec3 translation = Vec3(-1.5, 3, 7);

    vector<Vec3> vertices;
    vertices.push_back(translation + Vec3(1, 1, 1));
    vertices.push_back(translation + Vec3(-1, 1, 1));
    vertices.push_back(translation + Vec3(-1, -1, 1));
    vertices.push_back(translation + Vec3(1, -1, 1));
    vertices.push_back(translation + Vec3(1, 1, -1));
    vertices.push_back(translation + Vec3(-1, 1, -1));
    vertices.push_back(translation + Vec3(-1, -1, -1));
    vertices.push_back(translation + Vec3(1, -1, -1));

    Model cube = Model(vertices, Vec3(-1.5, 3, 7), RED);

    renderer.render_object(triangles, image);


    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}