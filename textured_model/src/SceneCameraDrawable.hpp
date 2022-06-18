#ifndef SCENE_CAMERA_DRAWABLE_HPP
#define SCENE_CAMERA_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;
class SceneCameraDrawable : public Drawable
{
public:
    SceneCameraDrawable(Scene& scene);
private:
    void render() const override;
};

#endif //SCENE_CAMERA_DRAWABLE_HPP
