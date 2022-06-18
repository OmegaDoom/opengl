#include "SceneBehavior.hpp"

#include "Scene.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>

SceneBehavior::SceneBehavior(Scene& scene, Window& window)
        : m_scene{ scene }
        , m_window{ window }
{
}

void SceneBehavior::Process()
{
    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_SPACE))
    {
        m_scene.changeSubroutine(!bool(m_scene.getSubroutine()));
    }
}
