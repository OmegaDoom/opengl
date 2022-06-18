#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "SceneObject.hpp"

class Scene;
class Drawable : virtual public SceneObject
{
public:
    Drawable(Scene& scene)
        : m_scene(scene)
    {
    }

    void Render() const
    {
        render();
    }

    Scene& scene() const
    {
        return m_scene;
    }

private:
    virtual void render() const = 0;

    Scene& m_scene;
};

#endif //DRAWABLE_HPP
