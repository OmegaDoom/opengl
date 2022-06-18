#ifndef CUBE_DRAWABLE_HPP
#define CUBE_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include <memory>
#include <glm/mat4x4.hpp>

class ShaderProgram;
class Scene;

class CubeDrawable : public Drawable, public Updateable
{
public:
    CubeDrawable(const Scene& scene);
    ~CubeDrawable();
private:
    void update() override;
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    glm::mat4 m_model;
};

#endif //CUBE_DRAWABLE
