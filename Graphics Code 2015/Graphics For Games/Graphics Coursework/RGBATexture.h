/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
class RGBATexture : public GeneratedTexture
{
public:
  RGBATexture(float width, float height)
      : GeneratedTexture((GLsizei)width, (GLsizei)height)
  {
    BufferImage(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE);
  }

  virtual ~RGBATexture()
  {
  }
};
}