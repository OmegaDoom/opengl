#ifndef MESH_HPP
#define MESH_HPP

#include "Vertex.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

class Mesh
{
public:
    Mesh(const std::vector<vertex>& vertices, const std::vector<unsigned> indices);
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;
    Mesh(Mesh&& rhs);
    Mesh& operator = (Mesh&&);
    ~Mesh();
    void Render() const;
private:
    unsigned m_vbo;
    unsigned m_ebo;
    unsigned m_indexCount;
};

#endif //MESH_HPP
