//
// Created by treyb on 2025-02-14.
//

#ifndef DRAW_H
#define DRAW_H

#include "vec3.h"
#include "image.h"

class Draw {
public:
    void put_pixel(int x, int y, Color& c, Image& image) { image.image[x][y] = c; }

    void draw_line(Vec3 P0, Vec3 P1, Color c, Image& image) {
        if ( abs(P1.x - P0.x) > abs(P1.y - P0.y)) {
            //line is horizontalish
            if (P0.x > P1.x) { std::swap(P0, P1); }

            auto ys = interpolate(P0.x, P0.y, P1.x, P1.y);
            for (int x = P0.x; x <= P1.x; x++) {
                put_pixel(x, ys[x - P0.x], c, image);
            }
        } else {
            //line is verticalish
            if (P0.y > P1.y) { std::swap(P0, P1); }

            auto xs = interpolate(P0.y, P0.x, P1.y, P1.x);
            for (int y = P0.y; y <= P1.y; y++) {
                put_pixel(xs[y - P0.y], y, c, image);
            }
        }

    }
};

class Triangle : Draw {
public:

    Triangle() {}
    Triangle(Vec3 P0, Vec3 P1, Vec3 P2) : P0(P0), P1(P1), P2(P2) {}

    void draw_wireframe_triangle(Color& c, Image& image) {
        draw_line(P0, P1, c, image);
        draw_line(P1, P2, c, image);
        draw_line(P2, P0, c, image);
    }
    //redunant - does same thing as shaded triangle with h val of 1.
    /*
    void draw_filled_triangle(Vec2 P0, Vec2 P1, Vec2 P2, Color c, Image& image) {
        //sort the points so that  y0 <= y1 <= y2
        if (P1.y < P0.y) { swap(P1, P0); }
        if (P2.y < P0.y) { swap(P2, P0); }
        if (P2.y < P1.y) { swap(P2, P1); }

        //compute x coords of triangle edges
        auto x01 = interpolate(P0.y, P0.x, P1.y, P1.x);
        auto x12 = interpolate(P1.y, P1.x, P2.y, P2.x);
        auto x02 = interpolate(P0.y, P0.x, P2.y, P2.x);

        //concatenate short sides
        x01.pop_back();
        vector<double> x012;
        x012.reserve(x01.size() + x12.size());
        x012.insert(x012.end(), x01.begin(), x01.end());
        x012.insert(x012.end(), x12.begin(), x12.end());

        //determine which is left and which is right
        double m = floor(x012.size() / 2.0);
        vector<double> x_left, x_right;
        if (x02[m] < x012[m]) {
            x_left = x02;
            x_right = x012;
        } else {
            x_left = x012;
            x_right = x02;
        }

        //draw horizonal segments
        for (int y = P0.y; y <= P2.y; y++) {
            for (int x = x_left[y - P0.y]; x <= x_right[y - P0.y]; x++) {
                put_pixel(x, y, c, image);
            }
        }
    }
    */

    void draw_shaded_triangle(Color& c, Image& image) {
        //sort the points so that  y0 <= y1 <= y2
        if (P1.y < P0.y) { std::swap(P1, P0); }
        if (P2.y < P0.y) { std::swap(P2, P0); }
        if (P2.y < P1.y) { std::swap(P2, P1); }

        auto x01 = interpolate(P0.y, P0.x, P1.y, P1.x);
        auto h01 = interpolate(P0.y, P0.z, P1.y, P1.z);

        auto x12 = interpolate(P1.y, P1.x, P2.y, P2.x);
        auto h12 = interpolate(P1.y, P1.z, P2.y, P2.z);

        auto x02 = interpolate(P0.y, P0.x, P2.y, P2.x);
        auto h02 = interpolate(P0.y, P0.z, P2.y, P2.z);

        x01.pop_back();
        std::vector<double> x012;
        x012.reserve(x01.size() + x12.size());
        x012.insert(x012.end(), x01.begin(), x01.end());
        x012.insert(x012.end(), x12.begin(), x12.end());

        h01.pop_back();
        std::vector<double> h012;
        h012.reserve(h01.size() + h12.size());
        h012.insert(h012.end(), h01.begin(), h01.end());
        h012.insert(h012.end(), h12.begin(), h12.end());

        double m = floor(x012.size() / 2.0);
        std::vector<double> x_left, x_right, h_left, h_right;
        if (x02[m] < x012[m]) {
            x_left = x02;
            h_left = h02;

            x_right = x012;
            h_right = h012;
        } else {
            x_left = x012;
            h_left = h012;

            x_right = x02;
            h_right = h02;
        }

        for (int y = P0.y; y <= P2.y; y++) {
            auto x_l = x_left[y - P0.y];
            auto x_r = x_right[y - P0.y];

            auto h_segment = interpolate(x_l, h_left[y - P0.y], x_r, h_right[y - P0.y]);
            for (int x = x_l; x <= x_r; x++) {
                Color shaded_color = Color(c * h_segment[x - x_l]);
                put_pixel(x, y, shaded_color, image);
            }
        }
    }

    Vec3 P0, P1, P2;


};

#endif //DRAW_H
