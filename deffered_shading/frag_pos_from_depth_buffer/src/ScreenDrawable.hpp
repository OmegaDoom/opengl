#ifndef SCREEN_DRAWABLE_HH
#define SCREEN_DRAWABLE_HH

#include "Drawable.hpp"

class Scene;

class ScreenDrawable : public Drawable
{
public:
    ScreenDrawable(Scene& scene);
    ~ScreenDrawable();
private:
    void render() const override;
};

#endif //SCREEN_DRAWABLE_HH
