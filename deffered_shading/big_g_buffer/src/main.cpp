#include "Window.hpp"
#include "Scene.hpp"
#include "WindowBehavior.hpp"
#include "CameraBehavior.hpp"
#include "TextureManager.hpp"
#include "CubemapTexture.hpp"
#include "ShaderManager.hpp"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <filesystem>

int main(int argc, char* argv[])
{
    const auto path = std::filesystem::path(argv[0]);
    const auto new_path = path.parent_path().parent_path().stem().append(path.parent_path().stem().c_str());
    std::filesystem::current_path(new_path);
    //Window window(800, 600);
    Window window;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit failed" << std::endl;
        return 1;
    }

    auto& textureManager = TextureManager::instance();
    textureManager.AddEmplace<Texture2D>("none");
    textureManager.LoadTextureS("./resources/container2.png", "container2");
    textureManager.LoadTexture("./resources/container2specular.png", "container2specular");
    textureManager.LoadTexture("./resources/Pebbles_006_SD/Pebbles_006_NRM.jpg", "pebblesNormalMap", GL_REPEAT, GL_REPEAT);
    textureManager.LoadTexture("./resources/Pebbles_006_SD/Pebbles_006_DISP.png", "pebblesDisp", GL_REPEAT, GL_REPEAT);
    textureManager.LoadTextureS("./resources/Stone_04_UV_H_CM_1.jpg", "stone");
    textureManager.LoadTextureS("./resources/grass.png", "grass", GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    textureManager.LoadTextureS("./resources/Pebbles_006_SD/Pebbles_006_COLOR.jpg", "pebbles", GL_REPEAT, GL_REPEAT);
    textureManager.AddEmplace<Texture2D>("depthMap", 1024, 1024, GL_DEPTH_COMPONENT
                                , GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 

    textureManager.AddEmplace<CubemapTexture>("cubemapDepthMap", 1024, 1024, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT
                                , GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureManager["cubemapDepthMap"]());
    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    std::array<std::string, 6> skyboxFaces { "./resources/skybox/right.jpg"
                                           , "./resources/skybox/left.jpg"
                                           , "./resources/skybox/top.jpg"
                                           , "./resources/skybox/bottom.jpg"
                                           , "./resources/skybox/front.jpg"
                                           , "./resources/skybox/back.jpg" };

    textureManager.LoadTexture(skyboxFaces, "skybox", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    textureManager.AddEmplace<Texture2D>("framebufferTex0", 1, 1, GL_RGB16F, GL_LINEAR
                                , GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 
    textureManager.AddEmplace<Texture2D>("framebufferTex1", 1, 1, GL_RGBA16F, GL_LINEAR
                                , GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 
    textureManager.AddEmplace<Texture2D>("framebufferTex2", 1, 1, GL_RGBA, GL_LINEAR
                                , GL_LINEAR, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 

    textureManager.AddEmplace<Texture2D>("depth_and_stencil", 1, 1, GL_DEPTH_STENCIL, GL_NEAREST
                                , GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER); 

    //std::cout << std::hex << "texture result = " << glGetError() << std::endl;
    auto& shaderManager = ShaderManager::instance();
    shaderManager.LoadAll();

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
