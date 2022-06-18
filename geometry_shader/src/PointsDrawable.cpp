#include "PointsDrawable.hpp"

#include "Vertex.hpp"

#include <GL/glew.h>
#include <cstddef>

PointsDrawable::PointsDrawable(Scene& scene)
    : Drawable{ scene }
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    
    vertex vertices[] = {{{ -0.5f, -0.5f }, {1.f, 0.f, 0.f}}
                        , {{ -0.5f, 0.5f }, {0.f, 1.f, 0.f}}
                        , {{ 0.5f, 0.5f }, {0.f, 0.f, 1.f}}
                        , {{ 0.5f, -0.5f }, {1.f, 1.f, 0.f}}};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned indices[] = { 0, 1, 2, 3 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

PointsDrawable::~PointsDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void PointsDrawable::render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_POINTS, 4, GL_UNSIGNED_INT, 0);
}
