#ifndef MESH_HPP
#define MESH_HPP

#include "Vertex.hpp"
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

class Texture;
class ShaderProgram;

class Mesh
{
public:
    Mesh(const std::vector<tangent_vertex>& vertices, const std::vector<unsigned>& indices
        , const std::vector<std::string>& diffuseTextures, const std::vector<std::string>& specularTextures
        , const std::vector<std::string>& normalmapTextures);
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;
    Mesh(Mesh&& rhs);
    Mesh& operator = (Mesh&&);
    ~Mesh();
    void Render(ShaderProgram& shaderProgram) const;
private:
    unsigned m_vbo;
    unsigned m_ebo;
    unsigned m_indexCount;

    std::vector<const Texture*> m_diffuse;
    std::vector<const Texture*> m_specular;
    std::vector<const Texture*> m_normalmap;
};

#endif //MESH_HPP
