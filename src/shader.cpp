
#include "shader.h"

Shader::Shader()
{
    programID = 0;
}
Shader::Shader(const char *vertPath, const char *fragPath)
{
    programID = glCreateProgram();

    // Vert Shader
    std::ifstream vShaderFile(vertPath, std::ifstream::binary);
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::filebuf *fileBuf = vShaderFile.rdbuf();
    // determine size of pointer by moving vShaderFile.in pointer from position 0 to end of file buffer (vshaderFile.end pointer)
    std::size_t size = fileBuf->pubseekoff(0, vShaderFile.end, vShaderFile.in);
    // set the position of vShaderFile.in back to 0
    fileBuf->pubseekpos(0, vShaderFile.in);
    // add 1 to size so we can null terminate buffer
    char *vertBuffer = new char[size + 1];
    fileBuf->sgetn(vertBuffer, size);
    // null terminate buffer so we don't have junk in our trunk
    vertBuffer[size] = '\0';

    vShaderFile.close();
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertBuffer, NULL);
    glCompileShader(vertexShaderID);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment shader
    std::ifstream fShaderFile(fragPath, std::ifstream::binary);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fileBuf = fShaderFile.rdbuf();
    // determine size of pointer by moving fShaderFile.in pointer from position 0 to end of file buffer (fshaderFile.end pointer)
    size = fileBuf->pubseekoff(0, fShaderFile.end, fShaderFile.in);
    // set the position of fShaderFile.in back to 0
    fileBuf->pubseekpos(0, fShaderFile.in);
    // add 1 to size so we can null terminate buffer

    char *fragBuffer = new char[size + 1];

    fileBuf->sgetn(fragBuffer, size);
    // null terminate buffer so we don't have junk in our trunk

    fragBuffer[size] = '\0';

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragBuffer, NULL);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING SHADER PROGRAM FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    // CAN WE DO THIS?
    delete vertBuffer, fragBuffer;
}

uint32_t Shader::getProgramID()
{
    return programID;
}