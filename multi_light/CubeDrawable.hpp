#ifndef CUBE_DRAWABLE_HPP
#define CUBE_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include <memory>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class ShaderProgram;
class Scene;
class Texture2D;

class CubeDrawable : public Drawable, public Updateable
{
public:
    CubeDrawable(const Scene& scene, glm::vec3 pos, int material);
    ~CubeDrawable();
private:
    void update() override;
    void render() const override;

    std::unique_ptr<ShaderProgram> m_shaderProgram;
    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;

    glm::mat4 m_model;
    glm::mat4 m_lightTransform0;
    glm::mat4 m_lightTransform1;
    glm::vec3 m_pos;
    int m_material;

    const Texture2D *m_diffuse;
    const Texture2D *m_specular;
};

#endif //CUBE_DRAWABLE
