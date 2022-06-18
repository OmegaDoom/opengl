#include "ShaderManager.hpp"

#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>

namespace
{
    const char* SHADERS_DIRECTORY = "./shaders/";
}

ShaderManager& ShaderManager::instance()
{
    static ShaderManager instance;
    return instance;
}

void ShaderManager::LoadAll()
{
    auto folderPathLength = std::strlen(SHADERS_DIRECTORY);

    for(auto& file: std::filesystem::directory_iterator(SHADERS_DIRECTORY))
    {
        auto path = file.path().string();
        size_t lastIndex = path.find_last_of("."); 
        auto rawname = path.substr(folderPathLength, lastIndex - folderPathLength);
        std::cout << rawname << std::endl;
        if (m_programs.cend() != m_programs.find(rawname))
            continue;
        Load(rawname);
    }
}

void ShaderManager::Load(const std::string& name)
{
    std::string vertexShaderPath = SHADERS_DIRECTORY + name + ".vsh";
    std::string fragmentShaderPath = SHADERS_DIRECTORY + name + ".fsh";
    std::string geometryShaderPath = SHADERS_DIRECTORY + name + ".gsh";

    std::ifstream ifs_vs(vertexShaderPath);
    std::ifstream ifs_fs(fragmentShaderPath);
    std::ifstream ifs_gs(geometryShaderPath);

    std::string vs{ std::istreambuf_iterator<char>(ifs_vs), std::istreambuf_iterator<char>() };
    std::string fs{ std::istreambuf_iterator<char>(ifs_fs), std::istreambuf_iterator<char>() };

    if (!ifs_gs.is_open())
    {
        m_programs.emplace(std::piecewise_construct, std::forward_as_tuple(name)
                            , std::forward_as_tuple(Shader<GL_VERTEX_SHADER>(vs), Shader<GL_FRAGMENT_SHADER>(fs)));
    }
    else
    {
        std::string gs{ std::istreambuf_iterator<char>(ifs_gs), std::istreambuf_iterator<char>() };
        m_programs.emplace(std::piecewise_construct, std::forward_as_tuple(name)
                            , std::forward_as_tuple(Shader<GL_VERTEX_SHADER>(vs), Shader<GL_GEOMETRY_SHADER>(gs)
                                                    , Shader<GL_FRAGMENT_SHADER>(fs)));
    }
}

ShaderProgram& ShaderManager::getShaderProgram(const std::string& name)
{
    return m_programs.at(name);
}
