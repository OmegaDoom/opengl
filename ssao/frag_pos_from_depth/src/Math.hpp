#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>

namespace math
{
    glm::vec3 CalcTangent(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2 uv1
                          , const glm::vec2& uv2, const glm::vec2& uv3)
    {
        const auto edge1{p2 - p1};
        const auto edge2{p3 - p1};
        const auto deltaUV1{uv2 - uv1};
        const auto deltaUV2{uv3 - uv1};

        const auto f{1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y)};
        return {f * ( deltaUV2.y * edge1.x - deltaUV1.y * edge2.x ) 
                , f * ( deltaUV2.y * edge1.y - deltaUV1.y * edge2.y ) 
                , f * ( deltaUV2.y * edge1.z - deltaUV1.y * edge2.z )};
    }
}

#endif //MATH_HPP
