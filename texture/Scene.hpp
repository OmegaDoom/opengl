#ifndef SCENE_HPP
#define SCENE_HPP

#include "SceneObject.hpp"
#include "Drawable.hpp"
#include "Updateable.hpp"
#include <memory>
#include <string>
#include <vector>

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
private:
    template<typename T, typename ... Args>
    void MakeAndAddSceneObject(Args&&... args);
    std::vector<std::unique_ptr<SceneObject>> m_scene_objects;
    std::vector<std::reference_wrapper<Drawable>> m_drawables;
    std::vector<std::reference_wrapper<Updateable>> m_updateables;
};

#endif //SCENE_HPP
