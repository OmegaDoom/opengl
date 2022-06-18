#include "NestedDrawable.hpp"

#include "Scene.hpp"

NestedDrawable::NestedDrawable(Scene& scene, std::unique_ptr<Drawable> drawable) 
    : Drawable(scene)
    , m_drawable{std::move(drawable)}
{
}

void NestedDrawable::SetDrawable(std::unique_ptr<Drawable> drawable)
{
    m_drawable = std::move(drawable);
}

void NestedDrawable::render() const
{
    m_drawable->Render();
}
