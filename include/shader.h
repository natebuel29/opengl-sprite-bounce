#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

class Shader
{
private:
    uint32_t programID;

public:
    Shader();
    Shader(const char *vertPath, const char *fragPath);

    uint32_t getProgramID();
    void SetMat4(GLuint uniformId, glm::mat4 &mat4);

    // Will need to setup classes for setting uniforms
};
#endif