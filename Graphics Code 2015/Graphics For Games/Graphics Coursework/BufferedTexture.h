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
  BufferedTexture(GLsizei width, GLsizei height, GLenum format, GLint internalFormat, GLenum type);
  virtual ~BufferedTexture();
};
}