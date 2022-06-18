#ifndef SHADOW_MAP_DRAWABLE_HH
#define SHADOW_MAP_DRAWABLE_HH

#include "Drawable.hpp"

class Scene;
class Texture2D;

class ShadowMapDrawable : public Drawable
{
public:
    ShadowMapDrawable(Scene& scene, const Texture2D& texture);
    ~ShadowMapDrawable();
private:
    void render() const override;

    unsigned m_fbo;
    unsigned m_rbo;
};

#endif //SHADOW_MAP_DRAWABLE_HH
