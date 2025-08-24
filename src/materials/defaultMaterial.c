#include "materials/defaultMaterial.h"
#include "material.h"
#include "shader.h"
#include "texture.h"

static void DefaultMaterialBind(UniversalMaterial* mat, bool isForward) {
    DefaultMaterial* material = (DefaultMaterial*)mat;
    Shader shader = isForward ? material->base.forwardShader : material->base.deferredShader;
    ShaderUse(shader);
    TextureBind(material->albedo, 0);
    ShaderSetInteger(shader, "albedo", 0);
}

static void DefaultMaterialDestroy(UniversalMaterial* mat) {
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
    material->base.deferredShader = ShaderCreate("Shaders/default.vert", "shaders/default.frag");
    material->base.forwardShader = ShaderCreate("Shaders/default_forward.vert", "shaders/default_forward.frag");
    return material;
}
