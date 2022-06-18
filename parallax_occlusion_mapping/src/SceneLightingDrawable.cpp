#include "SceneLightingDrawable.hpp"

#include "Scene.hpp"
#include "DirectionalLightShaderProgram.hpp"
#include "SpotLightShaderProgram.hpp"
#include "PointLightShaderProgram.hpp"

#include <cstring>

namespace
{
    const unsigned MAX_POINT_LIGHTS = 8;

    struct Lighting
    {
        SpotLight spotLight;
        DirectionalLight directionalLight;
        int pointLightCount;
        PointLight pointLights[MAX_POINT_LIGHTS];
    };
};

SceneLightingDrawable::SceneLightingDrawable(Scene& scene)
    : Drawable(scene)
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Lighting), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

SceneLightingDrawable::~SceneLightingDrawable()
{
    glDeleteBuffers(1, &m_ubo);
}

void SceneLightingDrawable::update()
{
    Lighting lighting{ scene().getSpotLight(), scene().getDirectionalLight()
                      , static_cast<int>(scene().getPointLights().size()) };

    for (int i = 0; i < scene().getPointLights().size(); ++i)
    {
        lighting.pointLights[i] = scene().getPointLights()[i];
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    auto p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    std::memcpy(p, &lighting, sizeof(Lighting));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
} 

void SceneLightingDrawable::render() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_ubo);
}
