#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "SceneObject.hpp"

class Scene;
class Drawable : virtual public SceneObject
{
public:
    Drawable(const Scene& scene)
        : m_scene(scene)
    {
    }

    void Render() const
    {
        render();
    }

    const Scene& scene() const
    {
        return m_scene;
    }

private:
    virtual void render() const = 0;

    const Scene& m_scene;
};

#endif //DRAWABLE_HPP
