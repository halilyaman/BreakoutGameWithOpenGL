#include "Shader.h"

#include <iostream>

Shader& Shader::Use()
{
    glUseProgram(this->id);
    return *this;
}

void Shader::Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
    unsigned int vertex_shader, fragment_shader, geometry_shader;
    // vertex Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    CheckCompileErrors(vertex_shader, "VERTEX");
    // fragment Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    CheckCompileErrors(fragment_shader, "FRAGMENT");
    // if geometry shader source code is given, also compile geometry shader
    if (geometry_source != nullptr)
    {
        geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_shader, 1, &geometry_source, NULL);
        glCompileShader(geometry_shader);
        CheckCompileErrors(geometry_shader, "GEOMETRY");
    }
    // shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, vertex_shader);
    glAttachShader(this->id, fragment_shader);
    if (geometry_source != nullptr)
        glAttachShader(this->id, geometry_shader);
    glLinkProgram(this->id);
    CheckCompileErrors(this->id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    if (geometry_source != nullptr)
        glDeleteShader(geometry_shader);
}

void Shader::SetFloat(const char* name, float value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->id, name), value);
}
void Shader::SetInteger(const char* name, int value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->id, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->id, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool use_shader)
{
    if (use_shader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}


void Shader::CheckCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}