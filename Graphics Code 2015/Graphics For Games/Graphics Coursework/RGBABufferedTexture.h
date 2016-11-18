/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
  class RGBABufferedTexture : public BufferedTexture
  {
  public:
    RGBABufferedTexture(size_t width, size_t height)
      : BufferedTexture()
    {
      glTexImage2D(m_type, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }

    virtual ~RGBABufferedTexture()
    {
    }
  };
}