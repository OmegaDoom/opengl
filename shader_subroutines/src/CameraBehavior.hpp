#ifndef CAMERA_BEHAVIOR_HPP
#define CAMERA_BEHAVIOR_HPP

class Camera;
class Window;

class CameraBehavior
{
public:
    CameraBehavior(Camera& camera, Window& window);
    void Process();
private:
    Camera& m_camera;
    Window& m_window;
    float m_prevTime;
};

#endif //CAMERA_BEHAVIOR_HPP
