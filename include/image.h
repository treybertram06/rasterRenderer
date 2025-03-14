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

        //std::cout << "Image object created with width: " << image_width << " and height: " << image_height << std::endl;
    }

    ~Image() {
        cleanup();
    }


    void output_image() {
        convert_image_to_byte_array();
        update_texture();
        render_quad();
    }

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
    Color background = {0.392, 0.584, 0.929};
    unsigned char* byte_array;
    GLuint textureID;

    void convert_image_to_byte_array() {
        for (int i = 0; i < image_width; ++i) {
            for (int j = 0; j < image_height; ++j) {
                if (image[i][j] == blank) {
                    image[i][j] = background;
                }

                // Calculate the new position for a 90-degree clockwise rotation
                int new_i = image_height - 1 - j;
                int new_j = i;

                // Store the pixel data in the rotated position
                byte_array[(new_j * image_width + new_i) * 3 + 0] = static_cast<unsigned char>(image[i][j].x * 255.0f);
                byte_array[(new_j * image_width + new_i) * 3 + 1] = static_cast<unsigned char>(image[i][j].y * 255.0f);
                byte_array[(new_j * image_width + new_i) * 3 + 2] = static_cast<unsigned char>(image[i][j].z * 255.0f);

            }
        }
        // std::cout << "Image rotated by 90 degrees and converted to byte array." << std::endl;
    }

    void update_texture() {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, byte_array);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //std::cout << "Texture updated with image data." << std::endl;
    }

    void render_quad() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    }
};


#endif //IMAGE_H
