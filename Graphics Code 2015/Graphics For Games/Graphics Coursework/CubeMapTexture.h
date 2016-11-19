/** @file */

#pragma once

#include "ITexture.h"

namespace GraphicsCoursework
{
/**
 * @class CubeMapTexture
 * @brief Holds a texture that stroes a cube map image.
 */
class CubeMapTexture : public ITexture
{
public:
  CubeMapTexture();
  virtual ~CubeMapTexture();

  virtual bool LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags = 0);
};
}
