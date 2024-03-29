/** @file */

#pragma once

#include <GL/glew.h>
#include <SOIL.h>
#include <string>

#include "ITexture.h"

namespace GraphicsCoursework
{
/**
 * @class Texture
 * @brief Basic 2D texture loaded from a file using SOIL.
 */
class Texture : public ITexture
{
public:
  Texture();
  virtual ~Texture();

  virtual bool LoadFromFile(const std::string &filename, unsigned int flags = 0);
};
}
