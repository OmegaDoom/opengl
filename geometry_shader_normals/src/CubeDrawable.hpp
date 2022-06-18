#ifndef CUBE_DRAWABLE_HPP
#define CUBE_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include "DirectionalLight.hpp"
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Scene;
class Texture2D;

class CubeDrawable : public Drawable, public Updateable
{
public:
    CubeDrawable(Scene& scene);
    ~CubeDrawable();
    void SetModel(const glm::mat4& model);
private:
    void update() override;
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    glm::mat4 m_model;

    const Texture2D *m_diffuse;
    const Texture2D *m_specular;
};

#endif //CUBE_DRAWABLE
