#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <chrono>
#include <thread>

#include "include/GLFW/glfw3.h"

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

void error_callback(int error, const char* description) {
    cerr << "Error: " << description << endl;
}

int main() {

    if (!glfwInit()) {
        cerr << "GLFW failed to initialize.\n";
        return -1;
    }

    int image_width  = 1000;
    int image_height = 1000;

    //clog << "Image allocated.\n";
    Image image = Image(image_width, image_height);

    double viewport_width   = 2;
    double viewport_height = viewport_width * image_height / image_width;
    double viewport_info[4] = { double(image_width), double(image_height), viewport_width, viewport_height };
    double distance_to_projection_plane = 1;


    Camera camera = Camera(Vec3(0, 0, -5), Vec3(0, 0, 0));

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
    Model cube2 = Model(vertices, Vec3(1.0, -1, 7), RED);

    scene.add_model(cube);
    scene.add_model(cube2);

    const int frame_rate = 60;
    const chrono::milliseconds target_frame_duration(1000 / frame_rate);

    GLFWwindow* window = glfwCreateWindow(image_width, image_height, "OpenGL Window", NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    //this needs to be fixed and placed inside the infinite loop to render in realtime (I think??)
    scene.render(renderer, image, viewport_info);


    while (!glfwWindowShouldClose(window)) {
        auto frame_start_time = chrono::high_resolution_clock::now();

        image.output_image();


        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frame_end_time = chrono::high_resolution_clock::now();
        auto frame_duration = duration_cast<chrono::milliseconds>(frame_end_time - frame_start_time);

        if (frame_duration < target_frame_duration) {
            this_thread::sleep_for(target_frame_duration - frame_duration);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    image.cleanup();



    image.cleanup();

    return 0;
}