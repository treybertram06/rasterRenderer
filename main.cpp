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

struct GameState {
    Camera camera = Camera();

};
GameState game_state;

void error_callback(int error, const char* description) {
    cerr << "Error: " << description << endl;
}




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.x -= 0.1f;
    }
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.x += 0.1f;
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.y += 0.1f;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.y -= 0.1f;
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.z += 0.1f;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        game_state.camera.pos.z -= 0.1f;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static const double sensitivity = 0.1;
    static const double smoothing = 0.85;
    static const double dead_zone = 0.05;

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    double center_x = window_width / 2.0;
    double center_y = window_height / 2.0;

    double xoffset = xpos - center_x;
    double yoffset = ypos - center_y;

    glfwSetCursorPos(window, center_x, center_y);

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    if (fabs(xoffset) < dead_zone) xoffset = 0.0;
    if (fabs(yoffset) < dead_zone) yoffset = 0.0;

    static double smoothed_x = 0.0;
    static double smoothed_y = 0.0;
    smoothed_x = smoothed_x * smoothing + xoffset * (1.0 - smoothing);
    smoothed_y = smoothed_y * smoothing + yoffset * (1.0 - smoothing);

    game_state.camera.rotation.x += smoothed_x;
    game_state.camera.rotation.y += smoothed_y;
    game_state.camera.rotation.z = 0;
}



int main() {

    if (!glfwInit()) {
        cerr << "GLFW failed to initialize.\n";
        return -1;
    }

    int image_width  = 1000;
    int image_height = 1000;



    double viewport_width   = 2;
    double viewport_height  = viewport_width * image_height / image_width;
    double viewport_info[4] = { double(image_width), double(image_height), viewport_width, viewport_height };
    double distance_to_projection_plane = 1;


    game_state.camera = Camera(Vec3(0, 0, 0), Vec3(0, 0, 0), viewport_width, viewport_height);

    auto renderer = Renderer(game_state.camera);
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




    Model cube = Model(vertices, Vec3(-1.5, 3, 7), Vec3(0, 0, 0), 1.0, RED);
    Model cube2 = Model(vertices, Vec3(0.0, 0.0, 7), RED);

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
    glfwSwapInterval(1); //Enable VSync


    glfwSetWindowUserPointer(window, &game_state);
    glfwSetKeyCallback(window, key_callback);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);



    while (!glfwWindowShouldClose(window)) {
        auto frame_start_time = chrono::high_resolution_clock::now();

        Image image = Image(image_width, image_height);

        renderer.set_camera(game_state.camera);
        scene.render(renderer, image, viewport_info);
        image.output_image();
        //clog << game_state.camera.rotation.x << " " << game_state.camera.rotation.y << " " << game_state.camera.rotation.z << endl;



        glfwPollEvents();

        auto frame_end_time = chrono::high_resolution_clock::now();
        auto frame_duration = duration_cast<chrono::milliseconds>(frame_end_time - frame_start_time);


        if (frame_duration < target_frame_duration) {
            this_thread::sleep_for(target_frame_duration - frame_duration);
        }

    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}

