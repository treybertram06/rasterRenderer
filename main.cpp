#include <iostream>
#include "include/color.h"
#include "include/image.h"
using namespace std;


void put_pixel(int x, int y, Color& c, Image& image) { image.image[x][y] = c; }

void draw_line(pair<int, int> P0, pair<int, int> P1, Color c, Image image) {
    double a = double(P1.second - P0.second) / double(P1.first - P1.first);
    double b = P0.second - a * P0.first;

    for (int x = P0.first; x <= P1.first; x++) {
        double y = a*x + b;
        put_pixel(x, y, c, image);
    }

}





int main() {

    int image_width  = 100;
    int image_height = 100;

    Image image = Image(image_width, image_height);

    //draw_line({15, 15}, {75, 75}, {255, 0, 0}, image);

    Color red = Color(255, 0, 0);
    put_pixel(50, 50, red, image);

    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}