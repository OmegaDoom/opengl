#ifndef SCENE_HPP
#define SCENE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "PointLight.hpp"
#include "SceneState.hpp"
#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

class TriangleDrawable;

class Scene
{
public:
	class exception : public std::exception
	{
	public:
		exception(std::string message)
			: m_message(std::move(message))
		{
		}

		const char* what() const noexcept override
		{
			return m_message.c_str();
		}
	private:
		std::string m_message;
	};

    Scene();
    void Update();
    void Render() const;
    void ChangeDimension(int width, int height);
    Camera& getCamera();
    const Camera& getCamera() const;
    const glm::mat4& Projection() const; 
    const DirectionalLight& getDirectionalLight() const;
    const SpotLight& getSpotLight() const;
    const std::vector<PointLight>& getPointLights() const;
    SceneState& getSceneState();
private:
    template<typename T, typename ... Args>
    T& MakeAndAddSceneObject(Args&&... args);
    std::vector<std::unique_ptr<SceneObject>> m_scene_objects;
    std::vector<std::reference_wrapper<Drawable>> m_drawables;
    std::vector<std::reference_wrapper<Updateable>> m_updateables;
    TriangleDrawable *m_triangle0;
    TriangleDrawable *m_triangle1;

    Camera m_camera;
    glm::mat4 m_projection;
    SceneState m_sceneState;
    DirectionalLight m_directionalLight;
    SpotLight m_spotLight;
    std::vector<PointLight> m_pointLights;
    std::vector<glm::vec3> m_lightRotationAxis;
    std::vector<glm::vec4> m_lightInitialPositions;
};

#endif //SCENE_HPP
