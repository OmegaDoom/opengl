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
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);
    glEnable(GL_FRAMEBUFFER_SRGB);
    //glDisable(GL_FRAMEBUFFER_SRGB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
