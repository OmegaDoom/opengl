#include "Window.hpp"
#include "Scene.hpp"
#include "WindowBehavior.hpp"
#include "CameraBehavior.hpp"
#include "TextureManager.hpp"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::filesystem::current_path(std::filesystem::path(argv[0]).stem());
    //Window window(1680, 1024);
    Window window;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit failed" << std::endl;
        return 1;
    }

    auto& textureManager = TextureManager::instance();
    textureManager.LoadTexture("./awesomeface.png", "awesomeface");

    Scene scene;

    scene.ChangeDimension(window.Width(), window.Height());
    window.DimensionChangedEvent([&scene] (auto width, auto height) { scene.ChangeDimension(width, height); });
    WindowBehavior windowBehavior(window);
    CameraBehavior cameraBehavior(scene.getCamera(), window);

    while(window.Process())
    {
        scene.Update();
        scene.Render();
        glfwPollEvents();
        cameraBehavior.Process();
        windowBehavior.Process();
    }
}
