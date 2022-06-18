#ifndef MODEL_DRAWABLE_HPP
#define MODEL_DRAWABLE_HPP

#include "Drawable.hpp"
#include <vector>
#include <memory>

class Scene;
class Mesh;
class Texture2D;

class ModelDrawable : public Drawable
{
public:
    ModelDrawable(Scene& scene);
    ~ModelDrawable();
private:
    void render() const override;
    unsigned m_VAO;
    const Texture2D *m_diffuse;
    const Texture2D *m_specular;
    std::vector<Mesh> m_meshes;
};

#endif //MODEL_DRAWABLE_HPP
