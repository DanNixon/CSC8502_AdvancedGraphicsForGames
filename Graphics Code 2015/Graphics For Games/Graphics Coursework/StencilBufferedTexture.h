/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class StencilBufferedTexture : public BufferedTexture
{
public:
  StencilBufferedTexture(size_t width, size_t height)
      : BufferedTexture()
  {
    glTexImage2D(m_type, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
  }

  virtual ~StencilBufferedTexture()
  {
  }
};
}