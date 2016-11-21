/** @file */

#include "Texture.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new texture.
 */
Texture::Texture()
    : ITexture(GL_TEXTURE_2D)
{
}

Texture::~Texture()
{
}

/**
 * @copydoc ITexture::LoadFromFile
 */
bool Texture::LoadFromFile(const std::string &filename, unsigned int flags)
{
  m_textureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
  return Valid();
}
}
