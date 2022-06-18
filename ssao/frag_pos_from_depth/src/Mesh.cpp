#include "Mesh.hpp"

#include "TextureManager.hpp"
#include "NormalMapTextureMaterialShaderProgram.hpp"

#include "ShaderProgram.hpp"
#include "NormalMapTextureMaterial.hpp"

#include <GL/glew.h>
#include <iostream>

Mesh::Mesh(const std::vector<tangent_vertex>& vertices, const std::vector<unsigned>& indices
            , const std::vector<std::string>& diffuseTextures, const std::vector<std::string>& specularTextures
            , const std::vector<std::string>& normalmapTextures)
    : m_indexCount(indices.size())
{
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(tangent_vertex), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto& path : diffuseTextures)
    {
        m_diffuse.push_back(&TextureManager::instance()[path]);
    }

    for (auto& path : specularTextures)
    {
        m_specular.push_back(&TextureManager::instance()[path]);
    }

    for (auto& path : normalmapTextures)
    {
        m_normalmap.push_back(&TextureManager::instance()[path]);
    }
}

Mesh::Mesh(Mesh&& rhs)
    : m_vbo(rhs.m_vbo)
    , m_ebo(rhs.m_ebo)
    , m_indexCount(rhs.m_indexCount)
    , m_diffuse(std::move(rhs.m_diffuse))
    , m_specular(std::move(rhs.m_specular))
    , m_normalmap(std::move(rhs.m_normalmap))
{
    rhs.m_vbo = 0;
    rhs.m_ebo = 0;
    rhs.m_indexCount  = 0;
}

Mesh& Mesh::operator = (Mesh&& rhs)
{
    m_vbo = rhs.m_vbo;
    m_ebo = rhs.m_ebo;
    m_indexCount = rhs.m_indexCount;
    m_diffuse = std::move(rhs.m_diffuse);
    m_specular = std::move(rhs.m_specular);
    m_normalmap = std::move(rhs.m_normalmap);

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

void Mesh::Render(ShaderProgram& shaderProgram) const
{
    NormalMapTextureMaterial textureMaterial{ 0, 1, 1.f, 2 };
    NormalMapTextureMaterialShaderProgram(shaderProgram).Apply(textureMaterial);

    if (!m_diffuse.empty())
    {
        m_diffuse[0]->Use(GL_TEXTURE0);
    }

    if (!m_specular.empty())
    {
        m_specular[0]->Use(GL_TEXTURE1);
    }

    if (!m_normalmap.empty())
    {
        m_normalmap[0]->Use(GL_TEXTURE2);
    }

    glBindVertexBuffer(0, m_vbo, 0, sizeof(tangent_vertex));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}
