#ifndef SCENE_HPP
#define SCENE_HPP

#include "Drawable.hpp"
#include "UpdateableDrawable.hpp"
#include <memory>
#include <vector>
#include <string>

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
    void AddDrawable(std::unique_ptr<Drawable>&& drawable);
    void AddDrawable(std::unique_ptr<UpdateableDrawable>&& updateable_drawable);
    std::vector<std::unique_ptr<Drawable>> m_drawables;
    std::vector<std::reference_wrapper<UpdateableDrawable>> m_updateables;
};

#endif //SCENE_HPP
