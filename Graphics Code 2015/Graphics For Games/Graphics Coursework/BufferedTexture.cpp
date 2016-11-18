/** @file */

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
BufferedTexture::BufferedTexture()
    : ITexture(GL_TEXTURE_2D)
{
  glGenTextures(1, &m_textureID);

  glBindTexture(m_type, m_textureID);
  glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

BufferedTexture::~BufferedTexture()
{
}
}