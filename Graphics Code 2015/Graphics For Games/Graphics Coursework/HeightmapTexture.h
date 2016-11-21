/** @file */

#pragma once

#include "GeneratedTexture.h"

namespace GraphicsCoursework
{
class FractalBrownianMotion;

/**
 * @class HeightmapTexture
 * @brief Texture that stores single channel for displacement mapping.
 */
class HeightmapTexture : public GeneratedTexture
{
public:
  HeightmapTexture(GLsizei width, GLsizei height);
  virtual ~HeightmapTexture();

  void SetFromFBM(FractalBrownianMotion *fbm);

protected:
  float *m_yData; //!< Pointer to height array
};
}
