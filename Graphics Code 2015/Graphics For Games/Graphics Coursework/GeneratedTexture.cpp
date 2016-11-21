/** @file */

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new texture.
 * @param width Width of the texture (in texels)
 * @param height Height of the texture (in texels)
 */
GeneratedTexture::GeneratedTexture(GLsizei width, GLsizei height)
    : ITexture(GL_TEXTURE_2D)
    , m_width(width)
    , m_height(height)
{
  glGenTextures(1, &m_textureID);

  glBindTexture(m_type, m_textureID);
  glTexParameterf(m_type, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(m_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(m_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glBindTexture(m_type, 0);
}

GeneratedTexture::~GeneratedTexture()
{
}

/**
 * @brief Copies buffered data into the texture.
 * @param format Texture format
 * @param internalFormat Texture format
 * @param type Data type
 * @param data Pointer to image data
 */
void GeneratedTexture::BufferImage(GLenum format, GLint internalFormat, GLenum type, void *data)
{
  glBindTexture(m_type, m_textureID);
  glTexImage2D(m_type, 0, internalFormat, m_width, m_height, 0, format, type, data);
  glBindTexture(m_type, 0);
}
}
