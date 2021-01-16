#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // state
    unsigned int id_;
    // constructor
    Shader() { }
    // sets the current shader as active
    Shader& Use();
    // compiles the shader from given source code
    void Compile(const char* vertex_source, const char* fragment_source, const char* geometry_source = nullptr); // note: geometry source code is optional 
    // utility functions
    void SetFloat(const char* name, float value, bool use_shader = false);
    void SetInteger(const char* name, int value, bool use_shader = false);
    void SetVector2f(const char* name, float x, float y, bool use_shader = false);
    void SetVector2f(const char* name, const glm::vec2& value, bool use_shader = false);
    void SetVector3f(const char* name, float x, float y, float z, bool use_shader = false);
    void SetVector3f(const char* name, const glm::vec3& value, bool use_shader = false);
    void SetVector4f(const char* name, float x, float y, float z, float w, bool use_shader = false);
    void SetVector4f(const char* name, const glm::vec4& value, bool use_shader = false);
    void SetMatrix4(const char* name, const glm::mat4& matrix, bool use_shader = false);
private:
    // checks if compilation or linking failed and if so, print the error logs
    void CheckCompileErrors(unsigned int object, std::string type);
};

