//
// Created by treyb on 2025-02-12.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "color.h"

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
  }

  void output_image() {
    std::cout << "P3\n";
    std::cout << image_width << " " << image_height << std::endl;
    std::cout << "255\n";
      
    //Color color;
    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            if (image[i][j].r < 0 || image[i][j].g < 0 || image[i][j].g < 0) {
                image[i][j] = {0, 0, 0};
            }
            image[i][j].convert_to_int();
            std::cout << image[i][j].r << ' ' << image[i][j].g << ' ' << image[i][j].b << std::endl;
        }
    }
  }
  
  
  void cleanup() {
    for (int i = 0; i < image_height; i++) {
        delete[] image[i];
    }
    delete[] image;
  }
  
  int image_width;
  int image_height;
  
  Color** image;


};

#endif //IMAGE_H
