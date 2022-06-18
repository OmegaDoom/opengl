#ifndef CALLBACK_DRAWABLE_HPP
#define CALLBACK_DRAWABLE_HPP

#include "Drawable.hpp"
#include "Scene.hpp"

template<typename T>
class CallbackDrawable : public Drawable
{
public:
    CallbackDrawable(Scene& scene, T callback)
        : Drawable(scene)
        , m_callback{ callback }
    {
    }
    
    void render() const override
    {
        m_callback(scene());
    }

private:
    T m_callback;
};

#endif //CALLBACK_DRAWABLE_HPP
