#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include "Texture.hpp"

#include <exception>
#include <string>

class Texture2D : public Texture
{
public:
    Texture2D();
    Texture2D(const char* filename, unsigned internalFormat, unsigned minFilter, unsigned magFilter
              , unsigned wrapS, unsigned wrapT);
    Texture2D(const Texture2D& rhs) = delete;
    Texture2D& operator = (const Texture2D& rhs) = delete;
    ~Texture2D() override;
    void Use(unsigned samplerId) const override;
private:
    unsigned m_tex;
};

#endif //TEXTURE_2D_HPP
