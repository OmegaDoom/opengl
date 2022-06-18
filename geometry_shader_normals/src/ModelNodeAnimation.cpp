#include "ModelNodeAnimation.hpp"

#include <algorithm>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

namespace
{
    glm::quat getRotation(float time, const std::vector<AnimationRotationKey>& rotationKeys)
    {
        auto rotIter = std::lower_bound(rotationKeys.cbegin(), rotationKeys.cend()
                                        , time, [](const auto& key, const auto& time) { return key.Time() < time ; } );
        if (rotIter == rotationKeys.cend())
        {
            return !rotationKeys.empty() ? rotationKeys.back().Rotation() : glm::quat();
        }

        auto prevRotIter = std::prev(rotIter);

        if (prevRotIter == rotationKeys.cend())
        {
            return rotIter->Rotation();
        }

        auto diffTimeBetweenKeys = rotIter->Time() - prevRotIter->Time();
        auto diffTime = time - prevRotIter->Time();
        auto scalar = diffTime / diffTimeBetweenKeys;

        return glm::normalize(glm::slerp(prevRotIter->Rotation(), rotIter->Rotation(), scalar));
    } 

    glm::vec3 getPosition(float time, const std::vector<AnimationPositionKey>& positionKeys)
    {
        auto posIter = std::lower_bound(positionKeys.cbegin(), positionKeys.cend()
                                        , time, [](const auto& key, const auto& time) { return key.Time() < time ; } );
        if (posIter == positionKeys.cend())
        {
            return !positionKeys.empty() ? positionKeys.back().Position() : glm::vec3(0.f);
        }

        auto prevPosIter = std::prev(posIter);

        if (prevPosIter == positionKeys.cend())
        {
            return posIter->Position();
        }

        auto diffTimeBetweenKeys = posIter->Time() - prevPosIter->Time();
        auto diffTime = time - prevPosIter->Time();
        auto scalar = diffTime / diffTimeBetweenKeys;

        return glm::mix(prevPosIter->Position(), posIter->Position(), scalar);
    } 

    glm::vec3 getScale(float time, const std::vector<AnimationScaleKey>& scaleKeys)
    {
        auto scaleIter = std::lower_bound(scaleKeys.cbegin(), scaleKeys.cend()
                                        , time, [](const auto& key, const auto& time) { return key.Time() < time ; } );
        if (scaleIter == scaleKeys.cend())
        {
            return !scaleKeys.empty() ? scaleKeys.back().Scale() : glm::vec3(1.f, 1.f, 1.f);
        }

        auto prevScaleIter = std::prev(scaleIter);

        if (prevScaleIter == scaleKeys.cend())
        {
            return scaleIter->Scale();
        }

        auto diffTimeBetweenKeys = scaleIter->Time() - prevScaleIter->Time();
        auto diffTime = time - prevScaleIter->Time();
        auto scalar = diffTime / diffTimeBetweenKeys;

        return glm::mix(prevScaleIter->Scale(), scaleIter->Scale(), scalar);
    } 
}

ModelNodeAnimation::ModelNodeAnimation(std::string name)
    : m_name(std::move(name)) 
{
}

const std::string& ModelNodeAnimation::Name() const
{
    return m_name;
}

void ModelNodeAnimation::AddPositionKey(const AnimationPositionKey& key)
{
    m_positionKeys.push_back(key);
}

void ModelNodeAnimation::AddRotationKey(const AnimationRotationKey& key)
{
    m_rotationKeys.push_back(key);
}

void ModelNodeAnimation::AddScaleKey(const AnimationScaleKey& key)
{
    m_scaleKeys.push_back(key);
}

glm::mat4 ModelNodeAnimation::Transform(double time) const
{
    auto position = getPosition(time, m_positionKeys);
    auto rotation = getRotation(time, m_rotationKeys);
    auto scale = getScale(time, m_scaleKeys);

    auto scaleTransform = glm::scale(glm::mat4(1.f), scale);
    auto rotationTransform = glm::mat4_cast(rotation);
    auto translationTransform = glm::translate(glm::mat4(1.f), position);
    return translationTransform * rotationTransform * scaleTransform;
}
