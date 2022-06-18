#ifndef SCENE_CAMERA_DRAWABLE_HPP
#define SCENE_CAMERA_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"

class Scene;

class SceneCameraDrawable : public Drawable, public Updateable
{
public:
    SceneCameraDrawable(Scene& scene);
    ~SceneCameraDrawable();
private:
    void update() override;
    void render() const override;

    unsigned int m_ubo;
};

#endif //SCENE_CAMERA_DRAWABLE_HPP
