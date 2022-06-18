#ifndef TRIANGLE_DRAWABLE_HPP
#define TRIANGLE_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp>
#include <iostream>

class Scene;
class Texture2D;

class TriangleDrawable : public Drawable
{
public:
    TriangleDrawable(Scene& scene, int width, int height, const Texture2D& diffuse);
    TriangleDrawable(const TriangleDrawable&) = delete;
    TriangleDrawable(TriangleDrawable&&) = delete;
    void SetCoordinates(glm::vec2 point0, glm::vec2 point1, glm::vec2 point2)
    {
        m_point0 = point0;
        m_point1 = point1;
        m_point2 = point2;
    }

    void SetTextureCoordinates(glm::vec2 tex0, glm::vec2 tex1, glm::vec2 tex2)
    {
        m_tex0 = tex0;
        m_tex1 = tex1;
        m_tex2 = tex2;
    }

    void SetScreenSize(int width, int height)
    {
        m_screen_width = width;
        m_screen_height = height;
    }

private:
    void render() const override;

    const Texture2D& m_diffuse;
    int m_screen_width;
    int m_screen_height;
    glm::vec2 m_point0;
    glm::vec2 m_point1;
    glm::vec2 m_point2;
    glm::vec2 m_tex0;
    glm::vec2 m_tex1;
    glm::vec2 m_tex2;
};

#endif // TRIANGLE_DRAWABLE_HPP
