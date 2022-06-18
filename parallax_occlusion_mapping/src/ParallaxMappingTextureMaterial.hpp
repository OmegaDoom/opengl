#ifndef PARALLAX_MAPPING_TEXTURE_MATERIAL_HPP
#define PARALLAX_MAPPING_TEXTURE_MATERIAL_HPP

struct ParallaxMappingTextureMaterial
{
    int diffuseTextureId;
    int specularTextureId;
    int normalMappingTextureId;
    int dispTextureId;
    float shininess;
};

#endif //PARALLAX_MAPPING_TEXTURE_MATERIAL_HPP
