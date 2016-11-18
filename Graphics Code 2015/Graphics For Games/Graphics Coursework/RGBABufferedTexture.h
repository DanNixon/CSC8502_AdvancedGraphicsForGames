/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class RGBABufferedTexture : public BufferedTexture
{
public:
  RGBABufferedTexture(float width, float height)
      : BufferedTexture()
  {
    glTexImage2D(m_type, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  }

  virtual ~RGBABufferedTexture()
  {
  }
};
}