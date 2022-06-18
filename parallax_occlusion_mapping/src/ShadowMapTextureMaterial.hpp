#ifndef SHADOW_MAP_TEXTURE_MATERIAL_HPP
#define SHADOW_MAP_TEXTURE_MATERIAL_HPP

struct ShadowMapTextureMaterial
{
    int diffuseTextureId;
    int specularTextureId;
    float shininess;
    int shadowMapTextureId;
};

#endif //SHADOW_MAP_TEXTURE_MATERIAL_HPP
