#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class Drawable
{
public:
    void Render() const
    {
        render();
    }

private:
    virtual void render() const = 0;
};

#endif //DRAWABLE_HPP
