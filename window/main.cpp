#include "Window.hpp"
#include <GLFW/glfw3.h>

int main(int, char* [])
{
    Window window;
    while(window.process())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
