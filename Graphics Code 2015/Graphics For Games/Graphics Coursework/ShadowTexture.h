/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
/**
 * @class ShadowTexture
 * @brief Holds a texture that stores depth data from a framebuffer for shadow mapping.
 */
class ShadowTexture : public GeneratedTexture
{
public:
  /**
   * @brief Creates a new shadow texture.
   * @param dim Texture dimensions
   */
  ShadowTexture(GLsizei dim)
      : GeneratedTexture(dim, dim)
  {
    glBindTexture(m_type, m_textureID);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, dim, dim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glBindTexture(m_type, 0);
  }

  virtual ~ShadowTexture()
  {
  }
};
}
