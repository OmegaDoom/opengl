#include "TexturesDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "TexturesShaderProgram.hpp"
#include "Textures.hpp"

void TexturesDrawable::render() const
{
    //setting material
    TexturesShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_textures);

    for (int i = 0; i < m_textures.count; ++i)
    {
        m_tex[i]->Use(GL_TEXTURE0 + i);
    }
}
