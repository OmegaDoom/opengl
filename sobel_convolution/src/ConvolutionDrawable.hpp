#ifndef CONVOLUTION_DRAWABLE_HPP
#define CONVOLUTION_DRAWABLE_HPP

#include "Drawable.hpp"

class Scene;
class Texture2D;

class ConvolutionDrawable : public Drawable
{
public:
    ConvolutionDrawable(Scene& scene, int width, int height, const Texture2D& diffuse);
private:
    void render() const override;

    const Texture2D& m_diffuse;
    int m_width;
    int m_height;
};

#endif //CONVOLUTION_DRAWABLE_HPP
