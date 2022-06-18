#include "ScreenDrawable.hpp"
#include <GL/glew.h>

ScreenDrawable::ScreenDrawable(Scene& scene)
    : Drawable{ scene }
{
}

ScreenDrawable::~ScreenDrawable()
{
}

void ScreenDrawable::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_FRAMEBUFFER_SRGB);
    //glDisable(GL_FRAMEBUFFER_SRGB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
