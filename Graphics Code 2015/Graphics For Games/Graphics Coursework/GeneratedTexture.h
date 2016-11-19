/** @file */

#pragma once

#include <GL/glew.h>
#include <string>

#include "ITexture.h"

namespace GraphicsCoursework
{
/**
 * @class GeneratedTexture
 * @brief Holds a texture created from buffered data.
 */
class GeneratedTexture : public ITexture
{
public:
  GeneratedTexture(GLsizei width, GLsizei height);
  virtual ~GeneratedTexture();

  /**
   * @brief Gets the width of the texture.
   * @return Width
   */
  inline GLsizei Width() const
  {
    return m_width;
  }

  /**
   * @brief Gets the height of the texture.
   * @return Height
   */
  inline GLsizei Height() const
  {
    return m_height;
  }

  void BufferImage(GLenum format, GLint internalFormat, GLenum type, void *data = nullptr);

protected:
  const GLsizei m_width;  //!< Width of the texture
  const GLsizei m_height; //!< height of the texture
};
}
