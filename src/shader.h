#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
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

    // Will need to setup classes for setting uniforms
};
#endif