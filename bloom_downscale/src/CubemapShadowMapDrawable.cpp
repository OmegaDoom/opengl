#include "CubemapShadowMapDrawable.hpp"
#include "CubemapTexture.hpp"
#include <GL/glew.h>
#include <iostream>

CubemapShadowMapDrawable::CubemapShadowMapDrawable(Scene& scene, const CubemapTexture& texture)
    : Drawable{ scene }
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    if (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        std::cout << "framebuffer is complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CubemapShadowMapDrawable::~CubemapShadowMapDrawable()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteRenderbuffers(1, &m_rbo);
}

void CubemapShadowMapDrawable::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}
