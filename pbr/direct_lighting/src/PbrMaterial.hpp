#ifndef PBR_MATERIAL_HPP
#define PBR_MATERIAL_HPP

#include <glm/glm.hpp>

struct PbrMaterial
{
    glm::vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

#endif //PBR_MATERIAL_HPP
