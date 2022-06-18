#include "Mesh.hpp"

#include <GL/glew.h>
#include <iostream>

Mesh::Mesh(const std::vector<vertex>& vertices, const std::vector<unsigned> indices)
    : m_indexCount(indices.size())
{
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::Mesh(Mesh&& rhs)
{
    m_vbo = rhs.m_vbo;
    m_ebo = rhs.m_ebo;
    m_indexCount = rhs.m_indexCount;
    rhs.m_vbo = 0;
    rhs.m_ebo = 0;
    m_indexCount  = 0;
}

Mesh& Mesh::operator = (Mesh&& rhs)
{
    m_vbo = rhs.m_vbo;
    m_ebo = rhs.m_ebo;
    m_indexCount = rhs.m_indexCount;
    rhs.m_vbo = 0;
    rhs.m_ebo = 0;
    m_indexCount  = 0;

    return *this;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Mesh::Render() const
{
    glBindVertexBuffer(0, m_vbo, 0, sizeof(vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}
