#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <string>

class FileSystem
{
public:
    static std::string getDirectory(const std::string& path);
    static std::string getExtension(const std::string& path);
    static bool IsDDSExtension(const std::string& path);
    static const std::string& ResourcesDirectory();
    static std::string getPathToResource(const std::string& name);
private:
    inline static const std::string RESOURCES_DIRECTORY = "./resources/";
};

#endif //FILE_SYSTEM_HPP
