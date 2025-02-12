#include <iostream>
#include "include/color.h"
using namespace std;



void output_image(int iw, int ih, Color** image) {
    cout << "P3\n";
    cout << iw << " " << ih << endl;
    cout << "255\n";

    Color color;
    for (int i = 0; i < (iw*ih); i++) {

    }

    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            cout << image[i][j].r << ' ' << image[i][j].g << ' ' << image[i][j].b << endl;
        }
    }
}

void put_pixel(int x, int y, Color c, Color** image) { image[x][y] = c; }

void draw_line(pair<int, int> P0, pair<int, int> P1, Color c, Color** image) {
    double a = double(P1.second - P0.second) / double(P1.first - P1.first);
    double b = P0.second - a * P0.first;

    for (int x = P0.first; x <= P1.first; x++) {
        double y = a*x + b;
        put_pixel(x, y, c, image);
    }

}

Color** initialize_image(int iw, int ih) {
    Color** image = new Color*[ih];
    for (int i = 0; i < ih; i++) {
        image[i] = new Color[iw];
    }

    Color c = Color(0, 0, 0);
    for (int i = 0; i < ih; i++) {
        for (int j = 0; j < iw; j++) {
            image[i][j] = c;
        }
    }

    return image;
}

void cleanup(Color** image, int ih) {
    for (int i = 0; i < ih; i++) {
        delete[] image[i];
    }
    delete[] image;
}

int main() {

    int image_width  = 100;
    int image_height = 100;

    Color** image = initialize_image(image_width, image_height);

    //draw_line({15, 15}, {75, 75}, {255, 0, 0}, image);

    put_pixel(50, 50, {255, 0, 0}, image);

    clog << "calling output image\n";
    output_image(image_width, image_height, image);



    cleanup(image, image_height);

    return 0;
}