/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class DepthStencilBufferedTexture : public BufferedTexture
{
public:
  DepthStencilBufferedTexture(float width, float height)
      : BufferedTexture()
  {
    glTexImage2D(m_type, 0, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, NULL);
  }

  virtual ~DepthStencilBufferedTexture()
  {
  }
};
}