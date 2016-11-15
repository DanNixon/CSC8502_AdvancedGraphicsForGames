/** @file */

#pragma once

#include "ITexture.h"

namespace GraphicsCoursework
{
class CubeMapTexture : public ITexture
{
public:
  CubeMapTexture();
  virtual ~CubeMapTexture();

  virtual bool LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags = 0);
};
}