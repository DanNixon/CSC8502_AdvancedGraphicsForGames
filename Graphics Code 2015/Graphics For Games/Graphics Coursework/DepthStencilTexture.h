/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
/**
 * @class DepthStencilTexture
 * @brief Holds a texture that stores depth or stencil data from a framebuffer.
 */
class DepthStencilTexture : public GeneratedTexture
{
public:
  /**
   * @brief Creates a new texture for binding to depth or stencil buffers of an FBO.
   * @param width Width in texels
   * @param height Height in texels
   */
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
