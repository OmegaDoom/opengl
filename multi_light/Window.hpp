#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <exception>
#include <string>
#include <functional>

struct GLFWwindow;
struct GLFWmonitor;

class Window
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

    Window(); //fullscreen
	Window(int width, int height);
    int Width() const;
    int Height() const;
    void DimensionChangedEvent(std::function<void(int width, int height)> subscriber);
    void Close();
    bool Process();
    GLFWwindow* getPtr() const { return m_window; };
private:
    void init();
    void createWindow(int width, int height, GLFWmonitor* monitor);
    void processInput();
	GLFWwindow *m_window;
    std::function<void(int, int)> m_subscriber;
};

#endif //WINDOW_HPP
