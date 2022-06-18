#include "CompoundDrawable.hpp"

#include "Scene.hpp"

CompoundDrawable::CompoundDrawable(Scene& scene, std::initializer_list<std::reference_wrapper<Drawable>> drawables)
    : Drawable(scene)
    , m_drawables{ drawables }
{
}

void CompoundDrawable::render() const
{
    for(Drawable& drawable : m_drawables)
    {
        drawable.Render();
    }
}
