#ifndef CUBEMAP_SHADOW_MAP_DRAWABLE_HH
#define CUBEMAP_SHADOW_MAP_DRAWABLE_HH

#include "Drawable.hpp"

class Scene;
class CubemapTexture;

class CubemapShadowMapDrawable : public Drawable
{
public:
    CubemapShadowMapDrawable(Scene& scene, const CubemapTexture& texture);
    ~CubemapShadowMapDrawable();
private:
    void render() const override;

    unsigned m_fbo;
    unsigned m_rbo;
};

#endif //CUBEMAP_SHADOW_MAP_DRAWABLE_HH
