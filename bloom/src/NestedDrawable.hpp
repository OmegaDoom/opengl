#ifndef NESTED_DRAWABLE_HPP
#define NESTED_DRAWABLE_HPP

#include "Drawable.hpp"

#include <vector>
#include <memory>

class Scene;

class NestedDrawable : public Drawable
{
public:
    NestedDrawable(Scene& scene, std::unique_ptr<Drawable> drawable);
    void SetDrawable(std::unique_ptr<Drawable> drawable);
private:
    virtual void render() const override;

    std::unique_ptr<Drawable> m_drawable;
};

#endif //NESTED_DRAWABLE_HPP
