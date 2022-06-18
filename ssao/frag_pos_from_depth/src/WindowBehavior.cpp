#include "WindowBehavior.hpp"

#include "Window.hpp"
#include <GLFW/glfw3.h>

WindowBehavior::WindowBehavior(Window& window)
    : m_window(window)
{
}

void WindowBehavior::Process()
{
    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_ESCAPE))
    {
        m_window.Close();
    }
}
