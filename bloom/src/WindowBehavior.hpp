#ifndef WINDOW_BEHAVIOR_HPP
#define WINDOW_BEHAVIOR_HPP

class Window;

class WindowBehavior
{
public:
    WindowBehavior(Window& window);
    void Process();
private:
    Window& m_window;
};

#endif //WINDOW_BEHAVIOR_HPP
