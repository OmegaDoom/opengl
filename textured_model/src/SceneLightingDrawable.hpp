#ifndef SCENE_LIGHTING_DRAWABLE_HPP
#define SCENE_LIGHTING_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;
class SceneLightingDrawable : public Drawable
{
public:
    SceneLightingDrawable(Scene& scene);
private:
    void render() const override;
};

#endif //SCENE_LIGHTING_DRAWABLE_HPP
