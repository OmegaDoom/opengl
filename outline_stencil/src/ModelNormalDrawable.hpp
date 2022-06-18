#ifndef MODEL_NORMAL_DRAWABLE_HPP
#define MODEL_NORMAL_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/mat4x4.hpp>

class Scene;

class ModelDrawable : public Drawable
{
public:
    ModelDrawable(Scene& scene, glm::mat4x4 model);
private:
    void render() const override;

    glm::mat4x4 m_model;
};

#endif // MODEL_NORMAL_DRAWABLE_HPP
