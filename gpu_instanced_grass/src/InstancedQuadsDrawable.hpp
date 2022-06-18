#ifndef INSTANCED_QUADS_DRAWABLE_HPP
#define INSTANCED_QUADS_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp> 
#include <vector> 

class InstancedQuadsDrawable : public Drawable
{
public:
    InstancedQuadsDrawable(Scene& scene, float width, float length, const std::vector<glm::mat4>& instances);
    ~InstancedQuadsDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_instancedVbo;
    unsigned m_ebo;

    unsigned m_indexCount;
    std::vector<glm::mat4> m_instances;
};

#endif //INSTANCED_QUADS_DRAWABLE_HPP
