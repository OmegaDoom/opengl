#ifndef MODEL_NORMAL_TRAMSFORM_DRAWABLE_HPP
#define MODEL_NORMAL_TRAMSFORM_DRAWABLE_HPP

#include "Drawable.hpp"

#include <glm/glm.hpp> 

class Scene;

class ModelNormalTransformDrawable : public Drawable
{
public:
    ModelNormalTransformDrawable(Scene& scene, const glm::mat4& model);
    ~ModelNormalTransformDrawable();
private:
    void render() const override;

    glm::mat4 m_model;
};

#endif //MODEL_NORMAL_TRAMSFORM_DRAWABLE_HPP
