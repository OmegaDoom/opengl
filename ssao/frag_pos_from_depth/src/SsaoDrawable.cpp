#include "SsaoDrawable.hpp"

#include "Scene.hpp"
#include "TextureManager.hpp"
#include <random>

namespace
{
    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }
}

SsaoDrawable::SsaoDrawable(Scene& scene)
        : Drawable(scene)
{
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = (float)i / 64.0;
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_ssaoKernel.push_back(sample);
    }

    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                0.0f);
        ssaoNoise.push_back(noise);
    }

    auto& noiseTexture = static_cast<Texture2D&>(TextureManager::instance()["noiseTexture"]);
    m_noiseTexture = &noiseTexture;
    noiseTexture.Update(ssaoNoise.data(), GL_RGB, GL_FLOAT);
}

SsaoDrawable::~SsaoDrawable()
{
}

void SsaoDrawable::render() const
{
    auto& shaderProgram = *scene().getSceneState().shaderProgram;
    shaderProgram.SetUniform("texNoise", 3);
    m_noiseTexture->Use(GL_TEXTURE3);

    const float *flat_array = &m_ssaoKernel[0].x;
    shaderProgram.SetUniform3("samples", 3 * m_ssaoKernel.size(), flat_array);

    shaderProgram.SetUniform("width", scene().Width());
    shaderProgram.SetUniform("height", scene().Height());
}
