#include "materials/defaultmaterial.h"
#include "material.h"
#include "shader.h"
#include "texture.h"

static void DefaultMaterialBind(Material* mat, mat4* modelMatrix, size_t instanceCount, bool isForward) {
    DefaultMaterial* material = (DefaultMaterial*)mat;
    Shader shader = isForward ? material->base.forwardShader : material->base.deferredShader;
    ShaderUse(shader);

    TextureBind(material->albedo, 0);
    ShaderSetInteger(shader, "albedo", 0);

    if (instanceCount <= 1 && modelMatrix) {
        ShaderSetMat4(shader, "model", *modelMatrix);
        ShaderSetBool(shader, "useInstances", false);
    } else {
        ShaderSetBool(shader, "useInstances", true);
    }
}

static void DefaultMaterialDestroy(Material* mat) {
    DefaultMaterial* material = (DefaultMaterial*)mat;
    TextureDestroy(material->albedo);
    ShaderDestroy(material->base.deferredShader);
    ShaderDestroy(material->base.forwardShader);
    free(material);
}
 
DefaultMaterial* DefaultMaterialCreate(const char* albedoPath) {
    DefaultMaterial* material = (DefaultMaterial*)malloc(sizeof(DefaultMaterial));
    material->albedo = TextureCreate(albedoPath, true, true);
    material->base.bindFunc = DefaultMaterialBind;
    material->base.destroyFunc = DefaultMaterialDestroy;
    material->base.deferredShader = ShaderCreate("shaders/default.vert", "shaders/default.frag");
    material->base.forwardShader = ShaderCreate("shaders/default.vert", "shaders/default.frag");
    return material;
}
