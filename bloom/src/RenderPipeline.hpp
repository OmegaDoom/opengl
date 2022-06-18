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
        PASS1 = 2,
        PASS2 = 4,
        PASS3 = 8,
        PASS4 = 16, 
        PASS5 = 32,
        PASS6 = 64, 
        PASS7 = 128, 
        PASS8 = 256, 
        PASS9 = 512,
        PASS10 = 1024, 
        PASS11 = 2048, 
        PASS12 = 4096, 
        PASS13 = 8192, 
        PASS14 = 16384, 
        PASS15 = 32768, 
        PASS16 = 65536 
    };

    void Add(const Drawable& drawable, int passes);
    void Render() const;

private:
    std::vector<std::vector<std::reference_wrapper<const Drawable>>> m_drawables;
};

#endif //RENDER_PIPELINE_HPP
