#ifndef PBR_MATERIAL_HPP
#define PBR_MATERIAL_HPP

#include <glm/glm.hpp>

struct PbrMaterial
{
    int albedo;
    int metallic;
    int roughness;
    int ao;
    int normal;
};

#endif //PBR_MATERIAL_HPP
