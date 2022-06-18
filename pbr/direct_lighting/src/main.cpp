#include "Window.hpp"
#include "Scene.hpp"
#include "WindowBehavior.hpp"
#include "CameraBehavior.hpp"
#include "SceneBehavior.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    const auto path = std::filesystem::path(argv[0]);
    const auto new_path = path.parent_path().parent_path().stem().append(path.parent_path().stem().c_str());
    std::filesystem::current_path(new_path);
    std::cout << std::filesystem::current_path() << std::endl;
    //Window window(800, 600);
    Window window;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit failed" << std::endl;
        return 1;
    }

    auto& textureManager = TextureManager::instance();

    auto& shaderManager = ShaderManager::instance();
    shaderManager.Load("textureMaterial");
    shaderManager.Load("pbr_material");

    Scene scene;

    scene.ChangeDimension(window.Width(), window.Height());
    window.DimensionChangedEvent([&scene] (auto width, auto height) { scene.ChangeDimension(width, height); });
    WindowBehavior windowBehavior(window);
    CameraBehavior cameraBehavior(scene.getCamera(), window);
    SceneBehavior sceneBehavior{scene, window};

    while(window.Process())
    {
        glfwPollEvents();
        cameraBehavior.Process();
        windowBehavior.Process();
        sceneBehavior.Process();
        scene.Update();
        scene.Render();
    }
}
