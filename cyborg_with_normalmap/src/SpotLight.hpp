#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include <glm/glm.hpp>

struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#endif //SPOT_LIGHT_HPP
