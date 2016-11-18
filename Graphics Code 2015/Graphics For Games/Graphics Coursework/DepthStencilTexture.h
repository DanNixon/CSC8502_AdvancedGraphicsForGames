/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
class DepthStencilTexture : public GeneratedTexture
{
public:
  DepthStencilTexture(float width, float height)
      : GeneratedTexture((GLsizei)width, (GLsizei)height)
  {
    BufferImage(GL_DEPTH_STENCIL, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8);
  }

  virtual ~DepthStencilTexture()
  {
  }
};
}