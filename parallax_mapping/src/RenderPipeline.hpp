#ifndef RENDER_PIPELINE_HPP
#define RENDER_PIPELINE_HPP

#include <vector>
#include <functional>

class Drawable;

class RenderPipeline
{
public:
    enum PASS
    {
        PASS0 = 1,
        PASS1 = 2
    };

    void Add(const Drawable& drawable, int passes);
    void Render() const;

private:
    std::vector<std::vector<std::reference_wrapper<const Drawable>>> m_drawables;
};

#endif //RENDER_PIPELINE_HPP
