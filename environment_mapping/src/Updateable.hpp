#ifndef UPDATEABLE_HPP
#define UPDATEABLE_HPP

#include "SceneObject.hpp"

class Updateable : virtual public SceneObject
{
public:
    void Update()
    {
        update();
    }
private:
    virtual void update() = 0;
};

#endif // UPDATEABLE_HPP
