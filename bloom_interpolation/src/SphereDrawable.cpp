#include "SphereDrawable.hpp"

#include "Scene.hpp"
#include "Vertex.hpp"

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace
{
    const float STRAIGHT_TRIANGLE_HEIGHT_AND_HALF_BASE_RELATION = 0.57735f;

    void tesselate(const int index_point0, const int index_point1
                   , const int index_point2, const int tessDepth
                   , std::vector<glm::vec3>& points, std::vector<int>& indices)
    {
        const glm::vec3& point0 = points[index_point0];
        const glm::vec3& point1 = points[index_point1];
        const glm::vec3& point2 = points[index_point2];

        glm::vec3 point3 = 0.5f * (point0 + point1);
        glm::vec3 point4 = 0.5f * (point1 + point2);
        glm::vec3 point5 = 0.5f * (point2 + point0);

        points.push_back(point3);
        points.push_back(point4);
        points.push_back(point5);

        int index_point3 = points.size() - 3;
        int index_point4 = points.size() - 2;
        int index_point5 = points.size() - 1;

        if (!tessDepth)
        {
            indices.push_back(index_point0);
            indices.push_back(index_point3);
            indices.push_back(index_point5);

            indices.push_back(index_point3);
            indices.push_back(index_point1);
            indices.push_back(index_point4);

            indices.push_back(index_point4);
            indices.push_back(index_point2);
            indices.push_back(index_point5);

            indices.push_back(index_point3);
            indices.push_back(index_point4);
            indices.push_back(index_point5);
            return;
        }

        tesselate(index_point0, index_point3, index_point5, tessDepth - 1, points, indices);
        tesselate(index_point3, index_point1, index_point4, tessDepth - 1, points, indices);
        tesselate(index_point4, index_point2, index_point5, tessDepth - 1, points, indices);
        tesselate(index_point3, index_point4, index_point5, tessDepth - 1, points, indices);
    }
}

SphereDrawable::SphereDrawable(Scene& scene, float radius)
    : Drawable(scene)
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    std::vector<glm::vec3> points;

    static const float STRAIGHT_TRIANGLE_HALF_BASE = STRAIGHT_TRIANGLE_HEIGHT_AND_HALF_BASE_RELATION * 1.5f;

    points.push_back(glm::vec3{ -STRAIGHT_TRIANGLE_HALF_BASE, -0.5f, STRAIGHT_TRIANGLE_HALF_BASE }); 
    points.push_back(glm::vec3{ 0.f, 1.f, 0.f }); 
    points.push_back(glm::vec3{ STRAIGHT_TRIANGLE_HALF_BASE, -0.5f, STRAIGHT_TRIANGLE_HALF_BASE }); 
    points.push_back(glm::vec3{ 0.f, -0.5f, -STRAIGHT_TRIANGLE_HALF_BASE }); 

    std::vector<int> indices;

    static const int TESS_DEPTH = 4;

    tesselate(0, 2, 1, TESS_DEPTH, points, indices);
    tesselate(3, 0, 1, TESS_DEPTH, points, indices);
    tesselate(2, 3, 1, TESS_DEPTH, points, indices);
    tesselate(3, 2, 0, TESS_DEPTH, points, indices);

    std::vector<vertex> vertices;
    for (auto& point : points)
    {
        glm::vec3 normal = glm::normalize(glm::vec3{ point.x, point.y, point.z });
        glm::vec3 vert = radius * normal;

        vertices.push_back(vertex{ {vert.x, vert.y, vert.z}, {1.f, 1.f, 1.f}
                                    , {0.f, 0.f}, {normal.x, normal.y, normal.z}});
    }

    std::cout << indices.size() << std::endl;

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, coords)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, color)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex)
                            , reinterpret_cast<void*>(offsetof(vertex, tex_coords))); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex)
                            , reinterpret_cast<void*>(offsetof(vertex, normal_coords)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);

    m_index_count = indices.size();
}

SphereDrawable::~SphereDrawable()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void SphereDrawable::render() const
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
}
