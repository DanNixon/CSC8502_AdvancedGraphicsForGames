/** @file */

#pragma once

#include <GL/glew.h>
#include <string>

#include "ITexture.h"

namespace GraphicsCoursework
{
class GeneratedTexture : public ITexture
{
public:
  GeneratedTexture(GLsizei width, GLsizei height);
  virtual ~GeneratedTexture();

  inline GLsizei Width() const
  {
    return m_width;
  }

  inline GLsizei Height() const
  {
    return m_height;
  }

  void BufferImage(GLenum format, GLint internalFormat, GLenum type, void *data = nullptr);

protected:
  const GLsizei m_width;
  const GLsizei m_height;
};
}