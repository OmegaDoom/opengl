#include "Window.hpp"
#include "Scene.hpp"

int main(int argc, char* argv[])
{
    Window window;
    Scene scene;

    scene.ChangeDimension(window.Width(), window.Height());
    window.DimensionChangedEvent([&scene] (auto width, auto height) { scene.ChangeDimension(width, height); });

    while(window.process())
    {
        scene.Update();
        scene.Render();
    }
}
