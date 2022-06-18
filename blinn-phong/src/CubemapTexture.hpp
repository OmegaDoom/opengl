#ifndef CUBEMAP_TEXTURE_2D_HPP
#define CUBEMAP_TEXTURE_2D_HPP

#include "Texture.hpp"

#include <exception>
#include <string>
#include <array>

class CubemapTexture : public Texture
{
public:
    CubemapTexture(const std::array<std::string, 6>& filenames
                   , unsigned internalFormat, unsigned minFilter, unsigned magFilter
                   , unsigned wrapS, unsigned wrapT, unsigned wrapR);
    CubemapTexture(const CubemapTexture& rhs) = delete;
    CubemapTexture& operator = (const CubemapTexture& rhs) = delete;
    ~CubemapTexture() override;
    void Use(unsigned samplerId) const override;
private:
    unsigned m_tex;
};

#endif //CUBEMAP_TEXTURE_2D_HPP
