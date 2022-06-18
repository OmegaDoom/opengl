#include "Window.hpp"
#include "Scene.hpp"
#include "WindowBehavior.hpp"
#include "CameraBehavior.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <filesystem>

int main(int argc, char* argv[])
{
    std::filesystem::current_path(std::filesystem::path(argv[0]).stem());
    //Window window(800, 600);
    Window window;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit failed" << std::endl;
        return 1;
    }

    auto& textureManager = TextureManager::instance();
    textureManager.AddEmplace("none");
    textureManager.LoadTextureS("./resources/container2.png", "container2");
    textureManager.LoadTexture("./resources/container2specular.png", "container2specular");
    textureManager.LoadTextureS("./resources/Stone_04_UV_H_CM_1.jpg", "stone");
    textureManager.LoadTextureS("./resources/grass.png", "grass", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

    std::array<std::string, 6> skyboxFaces { "./resources/ame_nebula/purplenebula_rt.tga"
                                           , "./resources/ame_nebula/purplenebula_lf.tga"
                                           , "./resources/ame_nebula/purplenebula_up.tga"
                                           , "./resources/ame_nebula/purplenebula_dn.tga"
                                           , "./resources/ame_nebula/purplenebula_ft.tga"
                                           , "./resources/ame_nebula/purplenebula_bk.tga" };

    textureManager.LoadTexture(skyboxFaces, "skybox", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    auto& shaderManager = ShaderManager::instance();
    shaderManager.Load("material");
    shaderManager.Load("textureMaterial");
    shaderManager.Load("discardAlpha");
    shaderManager.Load("skybox");

    Scene scene;

    scene.ChangeDimension(window.Width(), window.Height());
    window.DimensionChangedEvent([&scene] (auto width, auto height) { scene.ChangeDimension(width, height); });
    WindowBehavior windowBehavior(window);
    CameraBehavior cameraBehavior(scene.getCamera(), window);

    while(window.Process())
    {
        glfwPollEvents();
        cameraBehavior.Process();
        windowBehavior.Process();
        scene.Update();
        scene.Render();
    }
}
