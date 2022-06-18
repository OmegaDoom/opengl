#ifndef BILLBOARDS_QUADS_DRAWABLE_HPP
#define BILLBOARDS_QUADS_DRAWABLE_HPP

#include "Drawable.hpp"
#include <glm/glm.hpp> 
#include <vector> 

class BillboardQuadsDrawable : public Drawable
{
public:
    BillboardQuadsDrawable(Scene& scene, float width, float length, const std::vector<glm::vec3>& instances);
    ~BillboardQuadsDrawable();
private:
    void render() const override;

    unsigned m_vao;
    unsigned m_vbo;

    std::vector<glm::vec3> m_instances;
};

#endif //BILLBOARDS_QUADS_DRAWABLE_HPP
