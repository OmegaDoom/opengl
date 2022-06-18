#include "ViewProjectionDrawable.hpp"

#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace
{
    struct ViewProjectionData
    {
        glm::mat4 view;
        glm::mat4 projection;
    };
}

ViewProjectionDrawable::ViewProjectionDrawable(Scene& scene, const glm::mat4& view, glm::mat4 projection)
    : Drawable(scene)
    , m_view{ view }
    , m_projection{ projection }
{
    glGenBuffers(1, &m_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ViewProjectionData), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ViewProjectionDrawable::~ViewProjectionDrawable()
{
    glDeleteBuffers(1, &m_ubo);
}

void ViewProjectionDrawable::update()
{
    ViewProjectionData sceneCameraData{ m_view, m_projection };
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    auto p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    std::memcpy(p, &sceneCameraData, sizeof(ViewProjectionData));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void ViewProjectionDrawable::render() const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
}
