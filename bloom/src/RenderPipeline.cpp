#include "RenderPipeline.hpp"

#include "Drawable.hpp"

void RenderPipeline::Add(const Drawable& drawable, const int passes)
{
    auto pass = passes;
    unsigned passIndex = 0;
    while(pass)
    {
        if (pass & 1)
        {
            if (m_drawables.size() <= passIndex)
            {
                m_drawables.resize(passIndex + 1);
            }

            m_drawables[passIndex].push_back(drawable);    
        }

        pass >>= 1;
        ++passIndex;
    }    
}

void RenderPipeline::Render() const
{
    for(auto& pass : m_drawables)
    {
        for(const Drawable& drawable : pass)
        {
            drawable.Render();
        }
    }
}
