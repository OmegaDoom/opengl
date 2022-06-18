#ifndef SCENE_LIGHTING_DRAWABLE_HPP
#define SCENE_LIGHTING_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"

class Scene;

class SceneLightingDrawable : public Drawable, public Updateable
{
public:
    SceneLightingDrawable(Scene& scene);
    ~SceneLightingDrawable();
private:
    void update() override;
    void render() const override;

    unsigned m_ubo;
};

#endif //SCENE_LIGHTING_DRAWABLE_HPP
