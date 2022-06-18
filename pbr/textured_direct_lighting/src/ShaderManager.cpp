#include "ShaderManager.hpp"

#include "Shader.hpp"
#include <iostream>
#include <cstring>
#include <fstream>

namespace
{
    const char* SHADERS_DIRECTORY = "./shaders/";
    #include "pbr/textured_direct_lighting/binary_constants.hpp"
}

ShaderManager& ShaderManager::instance()
{
    static ShaderManager instance;
    return instance;
}

ShaderManager::ShaderManager()
{
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, -1, "/Constants", std::strlen(data), data);
}

void ShaderManager::Load(const std::string& name)
{
    std::string vertexShaderPath = SHADERS_DIRECTORY + name + ".vsh";
    std::string fragmentShaderPath = SHADERS_DIRECTORY + name + ".fsh";

    std::ifstream ifs_vs(vertexShaderPath);
    std::ifstream ifs_fs(fragmentShaderPath);

    std::string vs{ std::istreambuf_iterator<char>(ifs_vs), std::istreambuf_iterator<char>() };
    std::string fs{ std::istreambuf_iterator<char>(ifs_fs), std::istreambuf_iterator<char>() };

    m_programs.emplace(std::piecewise_construct, std::forward_as_tuple(name)
                            , std::forward_as_tuple(Shader<GL_VERTEX_SHADER>(vs), Shader<GL_FRAGMENT_SHADER>(fs)));
}

ShaderProgram& ShaderManager::getShaderProgram(const std::string& name)
{
    return m_programs.at(name);
}
