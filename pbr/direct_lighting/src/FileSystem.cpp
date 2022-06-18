#include "FileSystem.hpp"

#include <algorithm>

std::string FileSystem::getDirectory(const std::string& path)
{
    auto found = path.find_last_of("/\\");
    return path.substr(0, found) + '/'; 
}

const std::string& FileSystem::ResourcesDirectory()
{
    return RESOURCES_DIRECTORY;
}

std::string FileSystem::getPathToResource(const std::string& name)
{
    auto hasDirectory = name.find_last_of("/\\") != std::string::npos;
    return hasDirectory ? name : FileSystem::ResourcesDirectory() + name;
}
