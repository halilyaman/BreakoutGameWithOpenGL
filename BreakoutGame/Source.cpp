#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace game_val
{
    float screen_width{ 640.0 };
    float screen_height{ 480.0 };
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(game_val::screen_width, game_val::screen_height, "Hello World", NULL, NULL);
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

    // set viewport
    glViewport(0, 0, game_val::screen_width, game_val::screen_height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(1.000f, 0.894f, 0.882f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}