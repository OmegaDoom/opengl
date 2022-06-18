#include "CratesDrawable.hpp"

#include "Scene.hpp"
#include "CubeDrawable.hpp"
#include <random>
#include <glm/gtc/matrix_transform.hpp>

CratesDrawable::CratesDrawable(Scene& scene)
    : Drawable(scene)
    , m_drawable(std::make_unique<CubeDrawable>(scene))
{
}

CratesDrawable::~CratesDrawable()
{
}

void CratesDrawable::render() const
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-10.f, 10.f);

    for (int i = 0; i < 160; ++i)
    {
        float x = distribution(generator);
        float y = distribution(generator);
        float z = distribution(generator);
        glm::vec3 v(x, y, z);
        auto transform = glm::translate(glm::mat4(1.f), v);
        m_drawable->SetModel(transform);
        m_drawable->Render();
    }
}

