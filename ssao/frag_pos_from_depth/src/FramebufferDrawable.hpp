#ifndef FRAME_BUFFER_DRAWABLE_HH
#define FRAME_BUFFER_DRAWABLE_HH

#include "Drawable.hpp"
#include "Texture2D.hpp"

#include <GL/glew.h>
#include <iostream>

class Scene;

class FramebufferDrawable : public Drawable
{
public:
    template<typename Cont>
    FramebufferDrawable(Scene& scene, const Cont& color_textures)
        : Drawable(scene)
    {
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        int tex_idx = 0;
        for (auto& texture : color_textures)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + tex_idx, GL_TEXTURE_2D, texture(), 0);
            ++tex_idx;
        }

        m_tex_count = tex_idx;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    template<typename Cont>
    FramebufferDrawable(Scene& scene, const Cont& color_textures, const Texture& depth_and_stencil)
        : FramebufferDrawable(scene, color_textures)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_and_stencil(), 0);
        auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        std::cout << std::hex << "framebuffer status = " << result << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~FramebufferDrawable();
private:
    void render() const override;

    unsigned m_fbo;
    unsigned m_rbo;
    unsigned m_tex_count;
};

#endif //FRAME_BUFFER_DRAWABLE_HH
