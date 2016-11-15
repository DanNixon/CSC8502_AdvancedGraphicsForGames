/** @file */

#include "Texture.h"

namespace GraphicsCoursework
{
Texture::Texture()
    : ITexture()
{
}

Texture::~Texture()
{
}

bool Texture::LoadFromFile(const std::string &filename, unsigned int flags)
{
  m_textureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
  return Valid();
}
}