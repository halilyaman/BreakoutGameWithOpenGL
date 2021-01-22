#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "ResourceManager.h"

#define GLFW_GAMEPAD_BUTTON_A   0
#define GLFW_GAMEPAD_BUTTON_B   1
#define GLFW_GAMEPAD_BUTTON_X   2
#define GLFW_GAMEPAD_BUTTON_Y   3
#define GLFW_GAMEPAD_BUTTON_LEFT_BUMPER   4
#define GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER   5
#define GLFW_GAMEPAD_BUTTON_BACK   6
#define GLFW_GAMEPAD_BUTTON_START   7
#define GLFW_GAMEPAD_BUTTON_GUIDE   8
#define GLFW_GAMEPAD_BUTTON_LEFT_THUMB   9
#define GLFW_GAMEPAD_BUTTON_RIGHT_THUMB   10
#define GLFW_GAMEPAD_BUTTON_DPAD_UP   11
#define GLFW_GAMEPAD_BUTTON_DPAD_RIGHT   12
#define GLFW_GAMEPAD_BUTTON_DPAD_DOWN   13
#define GLFW_GAMEPAD_BUTTON_DPAD_LEFT   14
#define GLFW_GAMEPAD_BUTTON_LAST   GLFW_GAMEPAD_BUTTON_DPAD_LEFT
#define GLFW_GAMEPAD_BUTTON_CROSS   GLFW_GAMEPAD_BUTTON_A
#define GLFW_GAMEPAD_BUTTON_CIRCLE   GLFW_GAMEPAD_BUTTON_B
#define GLFW_GAMEPAD_BUTTON_SQUARE   GLFW_GAMEPAD_BUTTON_X
#define GLFW_GAMEPAD_BUTTON_TRIANGLE   GLFW_GAMEPAD_BUTTON_Y


// GLFW function declarations 
void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

Game breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // init glew
    if (glewInit() != GLEW_OK) 
    {
        std::cout << "Glew could not be initialized." << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    // OpenGL configuration
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    breakout.Init();

    // delta time variables
    float delta_time = 0.0f;
    float last_frame = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // manage user input
        breakout.ProcessInput(delta_time);

        // update game state
        breakout.Update(delta_time);

        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        breakout.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    } 
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            breakout.keys_[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            breakout.keys_[key] = false;
        }
    }
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}