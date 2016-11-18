/** @file */

#pragma once

#include "BufferedTexture.h"

namespace GraphicsCoursework
{
class FractalBrownianMotion;

class HeightmapBufferedTexture : public BufferedTexture
{
public:
  HeightmapBufferedTexture(GLsizei width, GLsizei height);
  virtual ~HeightmapBufferedTexture();

  void SetFromFBM(FractalBrownianMotion *fbm);
};
}