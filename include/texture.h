#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>
#include "glad/glad.h"

typedef struct Texture {
    GLuint id;
    int width;
    int height;
    int channels;
    GLenum format;
    bool hasMipmap;

    GLenum wrapS;
    GLenum wrapT;
    GLenum minFilter;
    GLenum magFilter;

    const char* path;
} Texture;

Texture* TextureCreate(const char* path, bool flipImage, bool generateMipmap);
void TextureBind(const Texture* texture, GLuint textureUnit);
void TextureDestroy(Texture* texture);

#endif
