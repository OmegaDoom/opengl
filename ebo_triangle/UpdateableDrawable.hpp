#ifndef UPDATEABLE_DRAWABLE_HPP
#define UPDATEABLE_DRAWABLE_HPP

#include "Drawable.hpp"

class UpdateableDrawable : public Drawable
{
public:
    void Update()
    {
        update();
    }
private:
    virtual void update() = 0;
};

#endif // UPDATEABLE_DRAWABLE_HPP
