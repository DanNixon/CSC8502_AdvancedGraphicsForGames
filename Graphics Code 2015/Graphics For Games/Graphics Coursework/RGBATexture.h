/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
/**
 * @class RGBATexture
 * @brief Holds a texture that stores 4 channel colour data from a framebuffer.
 */
class RGBATexture : public GeneratedTexture
{
public:
  /**
   * @brief Creates a new 4 channel colour texture for binding to a colour buffer of an FBO.
   * @param width Width in texels
   * @param height Height in texels
   */
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
