#ifndef FRAME_BUFFER_DRAWABLE_HH
#define FRAME_BUFFER_DRAWABLE_HH

#include "Drawable.hpp"
#include "Texture2D.hpp"

class Scene;

class FramebufferDrawable : public Drawable
{
public:
    FramebufferDrawable(Scene& scene, int width, int height);
    ~FramebufferDrawable();
private:
    void render() const override;

    unsigned m_fbo;
    unsigned m_rbo;
};

#endif //FRAME_BUFFER_DRAWABLE_HH
