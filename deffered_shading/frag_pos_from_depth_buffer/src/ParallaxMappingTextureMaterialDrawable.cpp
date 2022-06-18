#include "ParallaxMappingTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "ParallaxMappingTextureMaterialShaderProgram.hpp"

ParallaxMappingTextureMaterialDrawable::ParallaxMappingTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                                                 , const Texture& specular, const Texture& normalMapping, const Texture& disp, float shininess)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_normalMapping{ normalMapping }
    , m_disp{ disp }
    , m_specular{ specular }
    , m_material{ 0, 1, 2, 3, shininess}
{
}

void ParallaxMappingTextureMaterialDrawable::render() const
{
    //setting material
    ParallaxMappingTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);

    m_diffuse.Use(GL_TEXTURE0);
    m_specular.Use(GL_TEXTURE1);
    m_normalMapping.Use(GL_TEXTURE2);
    m_disp.Use(GL_TEXTURE3);
}
