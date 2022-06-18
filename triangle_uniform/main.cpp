#include "Window.hpp"
#include "Scene.hpp"

int main(int argc, char* argv[])
{
    Window window(800, 600);
    //Window window;
    Scene scene;

    while(window.process())
    {
        scene.Update();
        scene.Render();
    }
}
