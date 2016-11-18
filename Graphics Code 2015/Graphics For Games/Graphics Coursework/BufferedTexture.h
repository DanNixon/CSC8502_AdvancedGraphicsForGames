/** @file */

#pragma once

#include <GL/glew.h>
#include <string>

#include "ITexture.h"

namespace GraphicsCoursework
{
class BufferedTexture : public ITexture
{
public:
  BufferedTexture();
  virtual ~BufferedTexture();
};
}