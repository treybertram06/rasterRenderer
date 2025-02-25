#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include "GLFW/glfw3.h"

#include "include/image.h"
#include "include/vec2.h"
#include "include/vec3.h"
#include "include/color.h"
#include "include/renderer.h"
#include "include/model.h"
#include "include/camera.h"


using namespace std;

const Color RED = Color(255, 0, 0);
const Color GREEN = Color(0, 255, 0);
const Color BLUE = Color(0, 0, 255);
const Color WHITE = Color(255, 255, 255);
const Color BLACK = Color(0, 0, 0);


int main() {

    if (!glfwInit()) {
        cerr << "GLFW failed to initialize.\n";
    }

    int image_width  = 1000;
    int image_height = 1000;

    GLFWwindow* window = glfwCreateWindow(image_width, image_height, "OpenGL Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POINTS);
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(0.0f, 0.0f); // Pixel at the center
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();





    //clog << "Image allocated.\n";
    Image image = Image(image_width, image_height);

    double viewport_width   = 2;
    double viewport_height = viewport_width * image_height / image_width;
    double viewport_info[4] = { double(image_width), double(image_height), viewport_width, viewport_height };
    double distance_to_projection_plane = 1;


    Camera camera = Camera(Vec3(0, 0, -5), Vec3(30, 0, 0));

    Renderer renderer = Renderer(camera);
    Scene scene;


    vector<Vec3> vertices;
    vertices.push_back(Vec3(1, 1, 1));
    vertices.push_back(Vec3(-1, 1, 1));
    vertices.push_back(Vec3(-1, -1, 1));
    vertices.push_back(Vec3(1, -1, 1));
    vertices.push_back(Vec3(1, 1, -1));
    vertices.push_back(Vec3(-1, 1, -1));
    vertices.push_back(Vec3(-1, -1, -1));
    vertices.push_back(Vec3(1, -1, -1));




    Model cube = Model(vertices, Vec3(-1.5, 3, 7), Vec3(45, 15, 60), 1.0, RED);
    Model cube2 = Model(vertices, Vec3(1.5, -1, 7), RED);

    scene.add_model(cube);
    scene.add_model(cube2);


    scene.render(renderer, image, viewport_info);



    clog << "Writing image to file...\n";
    image.output_image();



    image.cleanup();

    return 0;
}