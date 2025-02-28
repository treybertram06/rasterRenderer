//
// Created by treyb on 2025-02-12.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "color.h"
#include "GLFW/glfw3.h"
//Color** image;

class Image {
public:

    Image(int image_width, int image_height) : image_width(image_width), image_height(image_height) {
        image = new Color*[image_height];
        for (int i = 0; i < image_height; i++) {
            image[i] = new Color[image_width];
        }

        Color c = Color(0, 0, 0);
        for (int i = 0; i < image_height; i++) {
            for (int j = 0; j < image_width; j++) {
                image[i][j] = c;
            }
        }
        byte_array = new unsigned char[image_width * image_height * 3];


    }

    void output_image() {
        convert_image_to_byte_array();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawPixels(image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, byte_array);
        glfwSwapBuffers(glfwGetCurrentContext());
    }



/*
  void output_image() {
      std::cout << "P3\n";
      std::cout << image_width << " " << image_height << std::endl;
      std::cout << "255\n";
      Color b = {0, 0, 0};
      //Color color;
      for (int i = 0; i < image_height; i++) {
          for (int j = 0; j < image_width; j++) {
              if (image[i][j].r < 0 || image[i][j].g < 0 || image[i][j].g < 0) {
                  image[i][j] = {0, 0, 0};
              }

              if (image[i][j] == b ) { image[i][j] = background; }
              image[i][j].convert_to_int();
              std::cout << image[i][j].r << ' ' << image[i][j].g << ' ' << image[i][j].b << std::endl;
          }
      }
  }
  */




    void cleanup() {
        for (int i = 0; i < image_height; i++) {
            delete[] image[i];
        }
        delete[] image;
        delete[] byte_array;

    }

  int image_width;
  int image_height;

  Color** image;

    Color background = {100, 149, 237};

private:

    unsigned char* byte_array;

    void convert_image_to_byte_array() {
        for (int i = 0; i < image_height; ++i) {
            for (int j = 0; j < image_width; ++j) {
                byte_array[(i * image_width + j) * 3 + 0] = static_cast<unsigned char>(image[i][j].x * 255);
                byte_array[(i * image_width + j) * 3 + 1] = static_cast<unsigned char>(image[i][j].y * 255);
                byte_array[(i * image_width + j) * 3 + 2] = static_cast<unsigned char>(image[i][j].z * 255);
            }
        }
    }

};

#endif //IMAGE_H
