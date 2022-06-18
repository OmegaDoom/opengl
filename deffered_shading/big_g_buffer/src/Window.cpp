#include "Window.hpp"

#include <GLFW/glfw3.h>

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
}

Window::Window()
{
    init();
    auto monitor = glfwGetPrimaryMonitor();

    const auto mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    createWindow(mode->width, mode->height, monitor);    
}

Window::Window(int width, int height)
{
    init();
    createWindow(width, height, nullptr);
}

int Window::Width() const
{
    int width;
    int height;
    glfwGetWindowSize(m_window, &width, &height);
    return width; 
}

int Window::Height() const
{
    int width;
    int height;
    glfwGetWindowSize(m_window, &width, &height);
    return height; 
}

void Window::createWindow(int width, int height, GLFWmonitor *monitor)
{
    m_window = glfwCreateWindow(width, height, "opengl window", monitor, nullptr);

    if (!m_window)
    {
            throw exception("failed to create window");
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwMakeContextCurrent(m_window);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(m_window
        , [] (GLFWwindow *window, int width, int height)
    { 
        glViewport(0, 0, width, height);
        auto ptr = static_cast<Window*>(glfwGetWindowUserPointer(window));
        ptr->m_subscriber(width, height);
    });
}

void Window::Close()
{
    glfwSetWindowShouldClose(m_window, true);
}

bool Window::Process()
{
    if (!glfwWindowShouldClose(m_window))
    {
        glfwSwapBuffers(m_window);
        return true;
    }

    glfwDestroyWindow(m_window);
    return false;
}

void Window::DimensionChangedEvent(std::function<void(int width, int height)> subscriber)
{
    m_subscriber = subscriber;
}
