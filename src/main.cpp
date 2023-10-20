#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #############################################################################
//                           Globals
// #############################################################################

float g_width = 768.0f;
float g_height = 768.0f;
bool g_initialized = false;

// #############################################################################
//                           Structs
// #############################################################################

struct Sprite
{
    int sizeX = 200;
    int sizeY = 300;
    float xPos;
    float yPos;
    float dx;
    float dy;
};

struct OpenGLContext
{
    GLuint vaoID;
    GLuint programID;
    GLuint textureID;
    GLuint projMatrixID;
    GLuint modelMatrixID;
    Shader shader;
};

// #############################################################################
//                           Functions
// #############################################################################
void glInit(OpenGLContext *glContext);

void window_size_callback(GLFWwindow *window, int width, int height);

void simulate(float dt, Sprite *sprite);
void render(float dt, OpenGLContext *glContext, Sprite *sprite);

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
    glEnable(GL_LINE_SMOOTH);

    OpenGLContext glContext = {};
    glInit(&glContext);

    Sprite sprite = {};
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        simulate(deltaTime, &sprite);
        /* Render here */
        render(deltaTime, &glContext, &sprite);
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
    // Shaders
    {
        Shader shader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
        glContext->shader = shader;
        glUseProgram(glContext->shader.getProgramID());
        glContext->programID = glContext->shader.getProgramID();
    }

    //     float verticies[] = {
    // // first triangle
    //       0.0f, 1.0f, 0.0f,                 0.0f, 1.0f,
    //      1.0f, 0.0f, 0.0f,                1.0f, 0.0f,
    //      0.0f, 0.0f, 0.0f,                0.0f, 0.0f,
    //  //      second triangle
    //    0.0f, 1.0f, 0.0f,                 0.0f, 1.0f,
    //       1.0f, 1.0f, 0.0f,               1.0f, 1.0f,
    //      1.0f, 0.0f, 0.0f                 1.0f, 0.0f,
    // };

    // my verticies won't lay out nice so :(
    float verticies[] = {
        // first triangle
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,
        //      second triangle
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
    };

    GLuint VBO;

    glGenVertexArrays(1, &glContext->vaoID);
    glBindVertexArray(glContext->vaoID);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture
    {
        // use stbi library to load image
        int width, height, nrChannels;
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

    // 2D projection matrix
    {
        glContext->projMatrixID = glGetUniformLocation(glContext->programID, "projection");
        glm::mat4 projection = glm::ortho(0.0f, g_width, g_height, 0.0f, -1.0f, 1.0f);
        glContext->shader.SetMat4(glContext->projMatrixID, projection);
    }

    // Model uniform
    {
        glContext->modelMatrixID = glGetUniformLocation(glContext->programID, "model");
    }
}

void simulate(float dt, Sprite *sprite)
{
    // initialize
    if (!g_initialized)
    {

        sprite->xPos = g_width / 2;
        sprite->yPos = g_height / 2;
        sprite->dx = -200.0f;
        sprite->dy = 40.0f;
        g_initialized = true;
    }
    // Update velocity
    sprite->xPos = sprite->xPos + (sprite->dx * dt);
    sprite->yPos = sprite->yPos + (sprite->dy * dt);

    // if off to left of screen
    if (sprite->xPos < 0)
    {
        sprite->dx = sprite->dx * -1;
    }
    // if off to right of screen
    else if (sprite->xPos + sprite->sizeX > g_width)
    {
        sprite->dx = sprite->dx * -1;
    }
    // if off of top of the screen
    if (sprite->yPos < 0)
    {
        sprite->dy = sprite->dy * -1;
    }
    // if off the bottom of the screen
    else if (sprite->yPos + sprite->sizeY > g_height)
    {
        sprite->dy = sprite->dy * -1;
    }
}

void render(float dt, OpenGLContext *glContext, Sprite *sprite)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite->xPos, sprite->yPos, 0.0f));
    model = glm::scale(model, glm::vec3(sprite->sizeX, sprite->sizeY, 0.0f));
    glContext->shader.SetMat4(glContext->modelMatrixID, model);

    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(glContext->shader.getProgramID());
    glBindVertexArray(glContext->vaoID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    g_width = (float)width;
    g_height = (float)height;
}