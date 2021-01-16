#include "ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::textures_;
std::map<std::string, Shader> ResourceManager::shaders_;


Shader ResourceManager::LoadShader(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file, std::string name)
{
    shaders_[name] = LoadShaderFromFile(v_shader_file, f_shader_file, g_shader_file);
    return shaders_[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return shaders_[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    textures_[name] = LoadTextureFromFile(file, alpha);
    return textures_[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
    return textures_[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : shaders_)
        glDeleteProgram(iter.second.id_);
    // (properly) delete all textures
    for (auto iter : textures_)
        glDeleteTextures(1, &iter.second.id_);
}

Shader ResourceManager::LoadShaderFromFile(const char* v_shader_file, const char* f_shader_file, const char* g_shader_file)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;
    try
    {
        // open files
        std::ifstream vertex_shader_file(v_shader_file);
        std::ifstream fragment_shader_file(f_shader_file);
        std::stringstream v_shader_stream, f_shader_stream;
        // read file's buffer contents into streams
        v_shader_stream << vertex_shader_file.rdbuf();
        f_shader_stream << fragment_shader_file.rdbuf();
        // close file handlers
        vertex_shader_file.close();
        fragment_shader_file.close();
        // convert stream into string
        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
        // if geometry shader path is present, also load a geometry shader
        if (g_shader_file != nullptr)
        {
            std::ifstream geometry_shader_file(g_shader_file);
            std::stringstream g_shader_stream;
            g_shader_stream << geometry_shader_file.rdbuf();
            geometry_shader_file.close();
            geometry_code = g_shader_stream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* v_shader_code = vertex_code.c_str();
    const char* f_shader_code = fragment_code.c_str();
    const char* g_shader_code = geometry_code.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(v_shader_code, f_shader_code, g_shader_file != nullptr ? g_shader_code : nullptr);
    return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.internal_format_ = GL_RGBA;
        texture.image_format_ = GL_RGBA;
    }
    // load image
    int width, height, nr_channels;
    unsigned char* data = stbi_load(file, &width, &height, &nr_channels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}