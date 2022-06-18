#include "SceneCameraDrawable.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>

namespace
{
    struct SceneCameraData
    {
        glm::mat4 view;
        glm::mat4 projection;
    };
}

SceneCameraDrawable::SceneCameraDrawable(Scene& scene)
    : Drawable(scene)
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneCameraData), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

SceneCameraDrawable::~SceneCameraDrawable()
{
    glDeleteBuffers(1, &m_ubo);
}

void SceneCameraDrawable::update()
{
    SceneCameraData sceneCameraData{ scene().getCamera().View(), scene().Projection() };
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    auto p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    std::memcpy(p, &sceneCameraData, sizeof(SceneCameraData));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void SceneCameraDrawable::render() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
}
