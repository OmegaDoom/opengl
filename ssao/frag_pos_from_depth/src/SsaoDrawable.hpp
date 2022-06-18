#ifndef SSAO_DRAWABLE_HPP
#define SSAO_DRAWABLE_HPP

#include "Drawable.hpp"
#include <vector>
#include <glm/vec3.hpp>

class Scene;
class Texture2D;

class SsaoDrawable : public Drawable
{
public:
    SsaoDrawable(Scene& scene);
    ~SsaoDrawable();
private:
    void render() const override;

    Texture2D *m_noiseTexture;
    std::vector<glm::vec3> m_ssaoKernel;
};

#endif // SSAO_DRAWABLE_HPP