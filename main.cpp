#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "include/image.h"
#include "include/vec2.h"
#include "include/vec3.h"
#include "include/color.h"
#include "include/draw.h"

using namespace std;

int main() {

    int image_width  = 1000;
    int image_height = 1000;

    Color red = Color(255, 0, 0);
    Color green = Color(0, 255, 0);
    Color blue = Color(0, 0, 255);
    Color white = Color(255, 255, 255);

    clog << "Image allocated.\n";
    Image image = Image(image_width, image_height);

    /*
    draw_line({10, 10}, {50, 900}, red, image);
    draw_line({100, 600}, {700, 50}, green, image);
    draw_line({350, 350}, {750, 200}, blue, image);
    draw_line({670, 10}, {5, 900}, white, image);
    */
    clog << "Drawing triangle to image buffer.\n";
    Triangle tri = Triangle({10, 10, 1}, {700, 50, 0.5}, {350, 900, 0});
    tri.draw_shaded_triangle(green, image);
    //draw_shaded_triangle({0, 0, 0}, {999, 0, 0.5}, {999, 999, 1}, green, image);
    //draw_wireframe_triangle({0, 0}, {999, 0}, {999, 999}, red, image);


    //draw_wireframe_triangle({10, 10}, {700, 50}, {350, 900}, red, image);


    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}