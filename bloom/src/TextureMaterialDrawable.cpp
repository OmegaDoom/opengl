#include "TextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "TextureMaterialShaderProgram.hpp"
#include "TextureMaterial.hpp"

TextureMaterialDrawable::TextureMaterialDrawable(Scene& scene, const Texture& diffuse
                                                 , const Texture& specular, float shininess)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_specular{ specular }
    , m_material{ 0, 1, shininess }
{
}

void TextureMaterialDrawable::render() const
{
    //setting material
    TextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);

    m_diffuse.Use(GL_TEXTURE0);
    m_specular.Use(GL_TEXTURE1);
}
