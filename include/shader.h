#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>
#include "glad/glad.h"

typedef GLuint Shader;

unsigned int ShaderCreate(const char* vertPath, const char* fragPath);
void ShaderDestroy(Shader shader);
void ShaderUse(Shader shader);

void ShaderSetMat4(Shader shader, const char* name, const mat4 mat);
void ShaderSetVec4v(Shader shader, const char* name, const vec4 vec);
void ShaderSetVec3v(Shader shader, const char* name, const vec3 vec);

void ShaderSetVec4(Shader shader, const char* name, float x, float y, float z, float a);
void ShaderSetVec3(Shader shader, const char* name, float x, float y, float z);

void ShaderSetBool(Shader shader, const char* name, const bool num);
void ShaderSetFloat(Shader shader, const char* name, const float num);
void ShaderSetInteger(Shader shader, const char* name, const int num);

#endif 
