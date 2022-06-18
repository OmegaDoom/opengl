#ifndef SCENE_BEHAVIOR_HPP
#define SCENE_BEHAVIOR_HPP

class Scene;
class Window;

class SceneBehavior
{
public:
    SceneBehavior(Scene& scene, Window& window);
    void Process();
private:
    Scene& m_scene;
    Window& m_window;
    float m_prevTime;
};


#endif // SCENE_BEHAVIOR_HPP
