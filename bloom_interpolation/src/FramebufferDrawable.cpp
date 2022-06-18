#include "FramebufferDrawable.hpp"

namespace
{
    const char* FRAMEBUFFER_NAME = "framebuffer";
}

FramebufferDrawable::~FramebufferDrawable()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
}

void FramebufferDrawable::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    //glEnable(GL_FRAMEBUFFER_SRGB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
