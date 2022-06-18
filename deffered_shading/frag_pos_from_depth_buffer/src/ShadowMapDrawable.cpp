#include "ShadowMapDrawable.hpp"

#include "Texture2D.hpp"
#include <GL/glew.h>
#include <iostream>

ShadowMapDrawable::ShadowMapDrawable(Scene& scene, const Texture2D& texture)
    : Drawable{ scene }
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        std::cout << "framebuffer is complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMapDrawable::~ShadowMapDrawable()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
}

void ShadowMapDrawable::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}
