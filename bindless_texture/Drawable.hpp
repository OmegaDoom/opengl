#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include "SceneObject.hpp"

class Drawable : virtual public SceneObject
{
public:
    void Render() const
    {
        render();
    }

private:
    virtual void render() const = 0;
};

#endif //DRAWABLE_HPP
