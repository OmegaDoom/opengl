#include "ConvolutionDrawable.hpp"

#include "Scene.hpp"
#include "Texture2D.hpp"
#include "ConvolutionShaderProgram.hpp"

ConvolutionDrawable::ConvolutionDrawable(Scene& scene, int width, int height, const Texture2D& diffuse)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_width{ width }
    , m_height{ height }
{
}

void ConvolutionDrawable::render() const
{
    ConvolutionShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_width, m_height, m_diffuse);

    m_diffuse.Use(GL_TEXTURE0);
}
