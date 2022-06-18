#ifndef CRATES_DRAWABLE_HPP
#define CRATES_DRAWABLE_HPP

#include "Drawable.hpp"
#include <memory>

class CubeDrawable;
class Scene;

class CratesDrawable : public Drawable
{
public:
    CratesDrawable(Scene& scene);
    ~CratesDrawable();
private:
    void render() const override;

    std::unique_ptr<CubeDrawable> m_drawable;
};

#endif //CRATES_DRAWABLE_HPP
