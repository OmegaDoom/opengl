#include "CameraBehavior.hpp"

#include "Camera.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <fstream>

namespace
{
    double last_xpos = 0;
    double last_ypos = 0;
    double cur_xpos = 0;
    double cur_ypos = 0;
    bool first_mouse = true;

    const float mouse_sensitivity = 0.02f;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        if (first_mouse)
        {
            last_xpos = xpos;
            last_ypos = ypos;
            first_mouse = false;
        }
        
        cur_xpos = xpos; 
        cur_ypos = ypos; 
    }
}

CameraBehavior::CameraBehavior(Camera& camera, Window& window)
    : m_camera{ camera }
    , m_window{ window }
{
    glfwSetCursorPosCallback(window.getPtr(), mouse_callback);
}

void CameraBehavior::Process()
{
    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_W))
    {
        m_camera.MoveForward();
    }

    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_S))
    {
        m_camera.MoveBack();
    }

    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_A))
    {
        m_camera.MoveLeft();
    }

    if (GLFW_PRESS == glfwGetKey(m_window.getPtr(), GLFW_KEY_D))
    {
        m_camera.MoveRight();
    }

    auto xpos_diff = last_xpos - cur_xpos;
    auto ypos_diff = cur_ypos - last_ypos;

    if (xpos_diff != 0.0)
    {
        m_camera.ChangeYaw(-xpos_diff * mouse_sensitivity);
    }

    if (ypos_diff != 0.0)
    {
        m_camera.ChangePitch(-ypos_diff * mouse_sensitivity);
    }

    last_xpos = cur_xpos;
    last_ypos = cur_ypos;
}
