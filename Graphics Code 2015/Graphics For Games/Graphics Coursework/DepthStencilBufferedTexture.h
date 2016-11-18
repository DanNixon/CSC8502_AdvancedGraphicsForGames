/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class DepthStencilBufferedTexture : public BufferedTexture
{
public:
  DepthStencilBufferedTexture(float width, float height)
      : BufferedTexture((GLsizei)width, (GLsizei)height, GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8)
  {
  }

  virtual ~DepthStencilBufferedTexture()
  {
  }
};
}