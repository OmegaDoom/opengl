#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace
{
    const auto SPEED_MULTIPLIER = 0.00002f;
    float getSpeed(std::chrono::time_point<std::chrono::steady_clock>& lastTime)
    {
        auto currentTime = std::chrono::steady_clock::now();
        return SPEED_MULTIPLIER * (std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count());
    }
}

Camera::Camera()
    : m_pos(0.f, 0.0f, 20.f)
    , m_front(0.f, 0.f, -1.f)
    , m_up(0.f, 1.f, 0.f)
    , m_view(1.f)
    , m_pitch(0.f)
    , m_yaw(-90.f)
    , m_lastTime(std::chrono::steady_clock::now())
{
}

void Camera::MoveForward()
{
    m_pos += getSpeed(m_lastTime) * m_front;
}

void Camera::MoveBack()
{
    m_pos -= getSpeed(m_lastTime) * m_front;
}

void Camera::MoveLeft()
{
    m_pos -= glm::normalize(glm::cross(m_front, m_up)) * getSpeed(m_lastTime);
}

void Camera::MoveRight()
{
    m_pos += glm::normalize(glm::cross(m_front, m_up)) * getSpeed(m_lastTime);
}

void Camera::Update()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_front = normalize(front);
    m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
    m_lastTime = std::chrono::steady_clock::now();
}

void Camera::ChangePitch(float grad)
{
    m_pitch += grad;
}

void Camera::ChangeYaw(float grad)
{
    m_yaw += grad;
}

const glm::mat4& Camera::View() const
{
    return m_view;
}
