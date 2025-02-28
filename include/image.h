#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "color.h"
#include "GLFW/glfw3.h"

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
        glGenTextures(1, &textureID);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Set the clear color to black

        std::cout << "Image object created with width: " << image_width << " and height: " << image_height << std::endl;
    }

    ~Image() {
        cleanup();
    }


    void output_image() {
        convert_image_to_byte_array();
        update_texture();
        render_quad();
        //get rid of this when you figure out how to render properly
        _sleep(500);
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
        glDeleteTextures(1, &textureID);
    }

    int image_width;
    int image_height;
    Color** image;

private:

    Color blank = {0, 0, 0};
    Color background = {100, 149, 237};
    unsigned char* byte_array;
    GLuint textureID;

    void convert_image_to_byte_array() {
        for (int i = 0; i < image_width; ++i) {
            for (int j = 0; j < image_height; ++j) {
                if (image[i][j] == blank) { image[i][j] = background; }
                byte_array[(j * image_height + i) * 3 + 0] = static_cast<unsigned char>(image[i][j].x);
                byte_array[(j * image_height + i) * 3 + 1] = static_cast<unsigned char>(image[i][j].y);
                byte_array[(j * image_height + i) * 3 + 2] = static_cast<unsigned char>(image[i][j].z);
                //std::clog << image[i][j].r << " " << image[i][j].g << " " << image[i][j].b << std::endl;

            }
        }
        //std::cout << "Image converted to byte array." << std::endl;
    }

    void update_texture() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, byte_array);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //std::cout << "Texture updated with image data." << std::endl;
    }

    void render_quad() {
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glfwSwapBuffers(glfwGetCurrentContext());
        //std::cout << "Rendered quad with texture." << std::endl;
    }
};

#endif //IMAGE_H
