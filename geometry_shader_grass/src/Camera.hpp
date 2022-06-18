#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <chrono>

class Camera
{
public:
    Camera();
    void MoveForward();
    void MoveBack();
    void MoveLeft();
    void MoveRight();
    void ChangePitch(float rad);
    void ChangeYaw(float rad);
    void Update();
    const glm::mat4& View() const;
    const glm::vec3& Front() const;
    const glm::vec3& Pos() const;
private:
    glm::mat4 m_view;
    glm::vec3 m_pos;
    glm::vec3 m_front;
    glm::vec3 m_up;
    float m_pitch;
    float m_yaw;
    std::chrono::time_point<std::chrono::steady_clock> m_lastTime;
};

#endif //CAMERA_HPP
