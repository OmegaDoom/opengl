#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include "Vertex.hpp"
#include <glm/glm.hpp>

namespace conv
{
    glm::vec3 convert(const vertex_coords& coords)
    {
        return {coords.x, coords.y, coords.z};
    }

    glm::vec2 convert(const vertex_tex_coords& coords)
    {
        return {coords.s, coords.t};
    }
}

#endif //CONVERSION_HPP
