#ifndef SCENE_HPP
#define SCENE_HPP

#include "Drawable.hpp"
#include "Updateable.hpp"
#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "PointLight.hpp"
#include "SceneState.hpp"
#include "RenderPipeline.hpp"
#include "Material.hpp"
#include <memory>
#include <vector>
#include <glm/mat4x4.hpp>

class NestedDrawable;

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
    int Width() const { return m_width; };
    int Height() const { return m_height; };
private:
    template<typename T, typename ... Args>
    T& MakeAndAddSceneObject(Args&&... args);
    template<typename T>
    T& AddSceneObject(std::unique_ptr<T>&& object);
    template<typename T>
    void AddSceneObject(T& object);
    std::vector<std::unique_ptr<SceneObject>> m_scene_objects;
    std::vector<std::reference_wrapper<Updateable>> m_updateables;
    RenderPipeline m_pipeline;
    
    Camera m_camera;
    glm::mat4 m_projection;
    SceneState m_sceneState;
    DirectionalLight m_directionalLight;
    SpotLight m_spotLight;
    std::vector<PointLight> m_pointLights;
    std::vector<glm::vec3> m_lightRotationAxis;
    std::vector<glm::vec4> m_lightInitialPositions;
    int m_width;
    int m_height;
    glm::mat4 m_lightView;

    std::vector<Material> m_colors;
};

#endif //SCENE_HPP
