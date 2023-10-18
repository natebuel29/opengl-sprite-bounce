#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
    GLuint textureID;
    Shader shader;
};

struct Transform
{
    int sizeX;
    int sizeY;
    float xPos;
    float yPos;
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
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

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

    //     float verticies[] = {
    // // first triangle
    //     0.25f, 0.55f, 0.0f,                 1.0f, 1.0f,
    //     0.25f, -0.25f, 0.0f,                1.0f, 0.0f,
    //     -0.25f, 0.55f, 0.0f,                0.0f, 1.0f,
    //  //      second triangle
    //     0.25f, -0.25f, 0.0f,                1.0f, 0.0f,
    //     -0.25f, -0.25f, 0.0f,               0.0f, 0.0f,
    //     -0.25f, 0.55f, 0.0f                 ,0.0f, 1.0f,
    // };

    // my verticies won't lay out nice so :(
    float verticies[] = {
        // first triangle
        0.25f,
        0.55f,
        0.0f,
        1.0f,
        1.0f,
        0.25f,
        -0.25f,
        0.0f,
        1.0f,
        0.0f,
        -0.25f,
        0.55f,
        0.0f,
        0.0f,
        1.0f,
        //      second triangle
        0.25f,
        -0.25f,
        0.0f,
        1.0f,
        0.0f,
        -0.25f,
        -0.25f,
        0.0f,
        0.0f,
        0.0f,
        -0.25f,
        0.55f,
        0.0f,
        0.0f,
        1.0f,
    };

    GLuint VBO;

    glGenVertexArrays(1, &glContext->vaoID);
    glBindVertexArray(glContext->vaoID);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture
    {
        // use stbi library to load image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *data = stbi_load("assets/textures/burnt_chili.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {

            glGenTextures(1, &glContext->textureID);
            glBindTexture(GL_TEXTURE_2D, glContext->textureID);
            glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
            glBindTexture(GL_TEXTURE_2D, glContext->textureID);
            // set the texture wrapping/filtering options (on the currently bound texture object)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    };
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}