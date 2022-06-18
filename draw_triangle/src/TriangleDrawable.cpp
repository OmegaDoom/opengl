#include "TriangleDrawable.hpp"

#include "Scene.hpp"
#include "Texture2D.hpp"
#include "TriangleShaderProgram.hpp"
#include <iostream>

TriangleDrawable::TriangleDrawable(Scene& scene, int width, int height, const Texture2D& diffuse)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_screen_width{width }
    , m_screen_height{height }
    , m_point0{ 0 }
    , m_point1{ 0 }
    , m_point2{ 0 }
    , m_tex0{ 0 }
    , m_tex1{ 0 }
    , m_tex2{ 0 }
{
}

void TriangleDrawable::render() const
{
    TriangleShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_screen_width, m_screen_height, m_point0, m_point1, m_point2, m_tex0, m_tex1, m_tex2, m_diffuse);

    m_diffuse.Use(GL_TEXTURE0);
}