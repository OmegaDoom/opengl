#include "Window.hpp"

#include <GLFW/glfw3.h>

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

Window::Window(int width, int height)
{
    init();
    createWindow(width, height, nullptr);
}

void Window::createWindow(int width, int height, GLFWmonitor *monitor)
{
	m_window = glfwCreateWindow(width, height, "opengl window", monitor, nullptr);

	if (!m_window)
	{
		throw exception("failed to create window");
	}

    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window
        , [] (GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); });
}

bool Window::process()
{
    if (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        processInput();
        glfwSwapBuffers(m_window);
        return true;
    }

    glfwDestroyWindow(m_window);
    glfwTerminate();
    return false;
}

void Window::processInput()
{
    if (GLFW_PRESS == glfwGetKey(m_window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(m_window, true);
    }
}
