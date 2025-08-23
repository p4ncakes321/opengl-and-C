#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static GLenum formats[5] = {
    0,         // index 0 unused
    GL_RED,    // 1 channel
    GL_RG,     // 2 channels
    GL_RGB,    // 3 channels
    GL_RGBA    // 4 channels
};

Texture* TextureCreate(const char *path, bool flipImage, bool generateMipmap) {
    Texture* tex = calloc(1, sizeof(Texture));
    if (!tex) {
        fprintf(stderr, "Failed to allocate Texture struct\n");
        return NULL;
    }

    tex->path = path;
    tex->hasMipmap = generateMipmap;

    glGenTextures(1, &tex->id);

    stbi_set_flip_vertically_on_load(flipImage);
    unsigned char* data = stbi_load(path, &tex->width, &tex->height, &tex->channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load image: %s\n", path);
        free(tex);
        return NULL;
    }

    if (tex->channels < 1 || tex->channels > 4) {
        fprintf(stderr, "Unsupported channel count: %s, %d\n", path, tex->channels);
        stbi_image_free(data);
        free(tex);
        return NULL;
    }

    tex->format = formats[tex->channels];

    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, tex->format, tex->width, tex->height,
                 0, tex->format, GL_UNSIGNED_BYTE, data);

    if (generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Defaults
    tex->wrapS = tex->wrapT = GL_REPEAT;
    tex->minFilter = generateMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
    tex->magFilter = GL_LINEAR;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->magFilter);

    stbi_image_free(data);

    return tex;
}

void TextureBind(const Texture* texture, GLuint textureUnit) {
    if (!texture || texture->id == 0) return;

#if defined(GL_VERSION_45)
    glBindTextureUnit(textureUnit, texture->id);
#else
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
#endif
}

void TextureDestroy(Texture* texture) {
    if (!texture) return;
    if (texture->id != 0) {
        glDeleteTextures(1, &texture->id);
    }
    free(texture);
}

