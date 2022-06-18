#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP

#include "ShaderProgram.hpp"
#include <unordered_map>
#include <string>

class ShaderManager
{
public:
    static ShaderManager& instance();
    void Load(const std::string& name);
    ShaderProgram& getShaderProgram(const std::string& name);
private:
    ShaderManager() = default;
    std::unordered_map<std::string, ShaderProgram> m_programs;
};

#endif //SHADER_MANAGER_HPP
