#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include <glm/glm.hpp>

struct SpotLight
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#endif //SPOT_LIGHT_HPP
