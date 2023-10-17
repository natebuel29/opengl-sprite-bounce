#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>

// #############################################################################
//                           Globals
// #############################################################################

// #############################################################################
//                           Structs
// #############################################################################

struct OpenGLContext
{
    GLuint vaoID;
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    Shader shader;
};

// #############################################################################
//                           Functions
// #############################################################################
void glInit(OpenGLContext *glContext);

void window_size_callback(GLFWwindow *window, int width, int height);

int main()
{

    GLFWwindow *window;

    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(768, 768, "Sprite Bounce", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    gladLoadGL();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 768, 768);

    OpenGLContext glContext = {};
    glInit(&glContext);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(glContext.shader.getProgramID());
        glBindVertexArray(glContext.vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    std::cout << "Init cmake and glfw" << std::endl;

    return 0;
}

void glInit(OpenGLContext *glContext)
{

    glContext->programID = glCreateProgram();

    // Shaders
    {
        Shader shader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
        glContext->shader = shader;
        glUseProgram(glContext->shader.getProgramID());
    }

    float verticies[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    GLuint VBO;

    glGenVertexArrays(1, &glContext->vaoID);
    glBindVertexArray(glContext->vaoID);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}