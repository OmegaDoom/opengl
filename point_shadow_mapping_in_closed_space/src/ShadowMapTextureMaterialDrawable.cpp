#include "ShadowMapTextureMaterialDrawable.hpp"

#include "Scene.hpp"
#include "Texture.hpp"
#include "ShadowMapTextureMaterialShaderProgram.hpp"
#include "ShadowMapTextureMaterial.hpp"

ShadowMapTextureMaterialDrawable::ShadowMapTextureMaterialDrawable(Scene& scene, const Texture& diffuse
                                                 , const Texture& specular, float shininess, const Texture& shadowMapTexture)
    : Drawable(scene)
    , m_diffuse{ diffuse }
    , m_specular{ specular }
    , m_shadowMap{ shadowMapTexture }
    , m_material{ 0, 1, shininess, 2 }
{
}

void ShadowMapTextureMaterialDrawable::render() const
{
    //setting material
    ShadowMapTextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);

    m_diffuse.Use(GL_TEXTURE0);
    m_specular.Use(GL_TEXTURE1);
    m_shadowMap.Use(GL_TEXTURE2);
}
