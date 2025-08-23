#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

typedef void (*GLGetivFunc)(GLuint, GLenum, GLint*);
typedef void (*GLGetInfoLogFunc)(GLuint, GLsizei, GLsizei*, GLchar*);

static int checkCompilationError(unsigned int instance, GLenum statusEnum, GLGetivFunc ivgetfunc, GLGetInfoLogFunc getinfofunc) {
    int success;
    char infoLog[512];
    ivgetfunc(instance, statusEnum, &success); 
    if (!success) {
        getinfofunc(instance, 512, NULL, infoLog);
        printf("Compilation failed:\n%s\n", infoLog);
        return 0;
    }
    return 1;
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

unsigned int ShaderCreate(const char *vertPath, const char *fragPath) {
    char* vertSource = readFile(vertPath);
    char* fragSource = readFile(fragPath);
    if (!vertSource || !fragSource) {
        free(vertSource);
        free(fragSource);
        return 0;
    }

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char* const*)&vertSource, NULL);
    glCompileShader(vertexShader);

    if (!checkCompilationError(vertexShader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog)) {
        goto cleanup;
    } 

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char* const*)&fragSource, NULL);
    glCompileShader(fragmentShader);

    if (!checkCompilationError(fragmentShader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog)) {
        goto cleanup;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    if (!checkCompilationError(shaderProgram, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog)) {
        glDeleteProgram(shaderProgram);
        goto cleanup;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertSource);
    free(fragSource);

    return shaderProgram;

cleanup:
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertSource);
    free(fragSource);
    return 0;
}

void ShaderDestroy(Shader shader) {
    glDeleteProgram(shader);
}

void ShaderUse(Shader shader) {
    glUseProgram(shader);
}

void ShaderSetMat4(Shader shader, const char* name, const mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, (const float*)mat);
}

void ShaderSetVec4v(Shader shader, const char *name, const vec4 vec) {
    glUniform4fv(glGetUniformLocation(shader, name), 1, (const float*)vec);  
}

void ShaderSetVec3v(Shader shader, const char *name, const vec3 vec) {
    glUniform3fv(glGetUniformLocation(shader, name), 1, (const float*)vec);
}

void ShaderSetVec4(Shader shader, const char *name, float x, float y, float z, float a) {
    glUniform4f(glGetUniformLocation(shader, name), x, y, z, a);
}

void ShaderSetVec3(Shader shader, const char *name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(shader, name), x, y, z);
}

void ShaderSetFloat(Shader shader, const char *name, const float num) {
    glUniform1f(glGetUniformLocation(shader, name), num);
}

void ShaderSetInteger(Shader shader, const char *name, const int num) {
    glUniform1i(glGetUniformLocation(shader, name), num);
}

void ShaderSetBool(Shader shader, const char *name, const bool num) {
    glUniform1i(glGetUniformLocation(shader, name), (int)num); 
}
