#ifndef MODEL_LOADER_HPP
#define MODEL_LOADER_HPP

#include <memory>

class ModelDrawable;
class Scene;

class ModelLoader
{
public:
    std::unique_ptr<ModelDrawable> Load(Scene& scene, const char* path);
};

#endif //MODEL_LOADER_HPP
