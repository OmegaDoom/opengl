#include "FileSystem.hpp"

#include <algorithm>

std::string FileSystem::getDirectory(const std::string& path)
{
    auto found = path.find_last_of("/\\");
    return path.substr(0, found) + '/'; 
}

std::string FileSystem::getExtension(const std::string& path)
{
    auto found = path.find_last_of(".");
    return path.substr(found + 1, std::string::npos); 
}

bool FileSystem::IsDDSExtension(const std::string& path)
{
    auto ext = FileSystem::getExtension(path); 
    if (3 != ext.size())
    {
        return false;
    }

    if (('d' == ext[0] || 'D' == ext[0])
        && ('d' == ext[1] || 'D' == ext[1])
        && ('s' == ext[2] || 'S' == ext[2]))
    {
        return true;
    }
    
    return false;
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
