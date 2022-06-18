#ifndef SCENE_HPP
#define SCENE_HPP

#include "Updateable.hpp"
#include "Drawable.hpp"
#include "Camera.hpp"
#include <memory>
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>

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
private:
    template<typename T, typename ... Args>
    void MakeAndAddSceneObject(Args&&... args);
    std::vector<std::unique_ptr<SceneObject>> m_scene_objects;
    std::vector<std::reference_wrapper<Drawable>> m_drawables;
    std::vector<std::reference_wrapper<Updateable>> m_updateables;
    
    Camera m_camera;
    glm::mat4 m_projection;
};

#endif //SCENE_HPP
