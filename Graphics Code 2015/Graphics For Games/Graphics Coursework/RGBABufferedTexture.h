/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class RGBABufferedTexture : public BufferedTexture
{
public:
  RGBABufferedTexture(float width, float height)
      : BufferedTexture((GLsizei)width, (GLsizei)height, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE)
  {
  }

  virtual ~RGBABufferedTexture()
  {
  }
};
}