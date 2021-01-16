#pragma once

#include <GL/glew.h>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int id_;
    // texture image dimensions
    unsigned int width_, height_; // width and height of loaded image in pixels
    // texture Format
    unsigned int internal_format_; // format of texture object
    unsigned int image_format_; // format of loaded image
    // texture configuration
    unsigned int wrap_s_; // wrapping mode on S axis
    unsigned int wrap_t_; // wrapping mode on T axis
    unsigned int filter_min_; // filtering mode if texture pixels < screen pixels
    unsigned int filter_max_; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};

