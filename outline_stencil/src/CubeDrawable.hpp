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
    CubeDrawable(Scene& scene, float size);
    ~CubeDrawable();
private:
    void update() override;
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_ebo;
};

#endif //CUBE_DRAWABLE
