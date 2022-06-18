#include "NormalMappingTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "NormalMappingTextureMaterialShaderProgram.hpp"

NormalMappingTextureMaterialDrawable::NormalMappingTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                                                 , const Texture& specular, float shininess, const Texture& normalMapping)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_specular{ specular }
    , m_normalMapping{ normalMapping }
    , m_material{ 0, 1, shininess, 2 }
{
}

void NormalMappingTextureMaterialDrawable::render() const
{
    //setting material
    NormalMappingTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);

    m_diffuse.Use(GL_TEXTURE0);
    m_specular.Use(GL_TEXTURE1);
    m_normalMapping.Use(GL_TEXTURE2);
}
