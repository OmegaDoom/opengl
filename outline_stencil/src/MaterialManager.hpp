#ifndef MATERIAL_MANAGER_HPP
#define MATERIAL_MANAGER_HPP

#include "Material.hpp"
#include "MaterialId.hpp"
#include <vector>

class MaterialManager
{
public:
    static MaterialManager& instance(); 
    const Material& operator[] (MATERIAL_ID) const;

private:
    MaterialManager();
    std::vector<Material> m_materials;
};

#endif //MATERIAL_MANAGER_HPP
