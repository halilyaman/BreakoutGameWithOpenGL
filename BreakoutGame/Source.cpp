#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "ResourceManager.h"


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

    int counter = 0;
    int sec(0);
    int last_sec(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        sec = static_cast<int>(glfwGetTime());
        counter++;
        if (sec > last_sec)
        {
            std::cout << "FPS: " << counter << std::endl;
            counter = 0;
        }
        last_sec = sec;
        counter++;

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