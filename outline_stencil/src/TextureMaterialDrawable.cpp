#include "TextureMaterialDrawable.hpp"
#include "TextureManager.hpp"
#include "Scene.hpp"
#include "TextureMaterialShaderProgram.hpp"
#include <GL/glew.h>

TextureMaterialDrawable::TextureMaterialDrawable(Scene& scene, const std::string& diffuse_map_name, const std::string& specular_map_name, float shiness)
    : Drawable(scene)
    , m_diffuse{TextureManager::instance()[diffuse_map_name]}
    , m_specular{TextureManager::instance()[specular_map_name]}
    , m_material{0, 1, shiness}
{
}

void TextureMaterialDrawable::render() const
{
    m_diffuse.Use(GL_TEXTURE0);
    m_specular.Use(GL_TEXTURE1);
    TextureMaterialShaderProgram(*scene().getSceneState().shaderProgram).Apply(m_material);
}

