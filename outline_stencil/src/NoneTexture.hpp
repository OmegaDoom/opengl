#ifndef NONE_TEXTURE_HPP
#define NONE_TEXTURE_HPP

#include "Texture.hpp"
#include <exception>
#include <string>

class NoneTexture : public Texture
{
public:
    void Use(unsigned int samplerId) const override;
};

#endif // NONE_TEXTURE_HPP
