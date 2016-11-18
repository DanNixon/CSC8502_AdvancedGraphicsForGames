/** @file */

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
BufferedTexture::BufferedTexture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLenum type)
    : ITexture(GL_TEXTURE_2D)
{
  glGenTextures(1, &m_textureID);

  glBindTexture(m_type, m_textureID);

  glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(m_type, 0, internalFormat, width, height, 0, format, type, NULL);

  glBindTexture(m_type, 0);
}

BufferedTexture::~BufferedTexture()
{
}
}