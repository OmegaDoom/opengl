#ifndef COMPOUND_DRAWABLE_HPP
#define COMPOUND_DRAWABLE_HPP

#include "Drawable.hpp"

#include <initializer_list>
#include <functional>
#include <vector>

class Scene;

class CompoundDrawable : public Drawable
{
public:
    CompoundDrawable(Scene& scene, std::initializer_list<std::reference_wrapper<Drawable>> drawables);
private:
    virtual void render() const override;

    std::vector<std::reference_wrapper<Drawable>> m_drawables;
};

#endif //COMPOUND_DRAWABLE_HPP