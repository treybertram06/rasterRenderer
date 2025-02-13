#include <iostream>
#include <vector>
#include "include/color.h"
#include "include/image.h"
using namespace std;


void put_pixel(int x, int y, Color& c, Image& image) { image.image[x][y] = c; }

vector<double> interpolate(int i0, int d0, int i1, int d1) {
    vector<double> values;
    if (i0 == i1) {
        values.push_back(d0);
        return values;
    }

    double a = (d1 - d0) / double(i1 - i0);
    double d = d0;

    for (int i = i0; i <= i1; i++) {
        values.push_back(d);
        d = d+a;
    }
    return values;

}

void draw_line(pair<int, int> P0, pair<int, int> P1, Color c, Image& image) {
    if ( abs(P1.first - P0.first) > abs(P1.second - P0.second)) {
        //line is horizontalish
        if (P0.first > P1.first) { swap(P0, P1); }

        auto ys = interpolate(P0.first, P0.second, P1.first, P1.second);
        for (int x = P0.first; x <= P1.first; x++) {
            put_pixel(x, ys[x - P0.first], c, image);
        }
    } else {
        //line is verticalish
        if (P0.second > P1.second) { swap(P0, P1); }

        auto xs = interpolate(P0.second, P0.first, P1.second, P1.first);
        for (int y = P0.second; y <= P1.second; y++) {
            put_pixel(y, xs[y - P0.second], c, image);
        }
    }

}


void draw_wireframe_triangle();



int main() {

    int image_width  = 1000;
    int image_height = 1000;

    Color red = Color(255, 0, 0);
    Color green = Color(0, 255, 0);
    Color blue = Color(0, 0, 255);
    Color white = Color(255, 255, 255);


    Image image = Image(image_width, image_height);

    /*
    draw_line({10, 10}, {50, 900}, red, image);
    draw_line({100, 600}, {700, 50}, green, image);
    draw_line({350, 350}, {750, 200}, blue, image);
    draw_line({670, 10}, {5, 900}, white, image);
    */

    //put_pixel(50, 50, red, image);

    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}