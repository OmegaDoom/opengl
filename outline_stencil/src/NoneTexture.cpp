#include "NoneTexture.hpp"

#include <GL/glew.h>

void NoneTexture::Use(unsigned int samplerId) const
{
    glActiveTexture(samplerId);
    glBindTexture(GL_TEXTURE_2D, 0);
}
