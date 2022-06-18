#include "Window.hpp"
#include "Scene.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::filesystem::current_path(std::filesystem::path(argv[0]).stem());

    Window window;
    Scene scene;

    while(window.process())
    {
        scene.Update();
        scene.Render();
    }
}
