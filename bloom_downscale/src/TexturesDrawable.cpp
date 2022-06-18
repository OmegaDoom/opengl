#include "TexturesDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "TexturesShaderProgram.hpp"
#include "Textures.hpp"

TexturesDrawable::TexturesDrawable(Scene& scene, std::initializer_list<std::reference_wrapper<const Texture>> textures)
    : Drawable(scene)
    , m_textures{0, {0, 1, 2, 3, 4, 5, 6, 7}}
{
    int texture_count = 0;
    for (auto&& texture : textures)
    {
        m_tex[texture_count] = &(texture.get());
        ++texture_count;
    }
    
    m_textures.count = texture_count;
}

void TexturesDrawable::render() const
{
    //setting material
    TexturesShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_textures);

    for (int i = 0; i < m_textures.count; ++i)
    {
        m_tex[i]->Use(GL_TEXTURE0 + i);
    }
}
