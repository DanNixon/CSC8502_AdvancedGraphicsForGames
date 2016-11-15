/** @file */

#pragma once

#include "..\nclgl\Mesh.h"

namespace GraphicsCoursework
{
class FractalBrownianMotion;

class HeightMapMesh : public Mesh
{
public:
  HeightMapMesh(float width, float depth, size_t widthSteps, size_t depthSteps);
  virtual ~HeightMapMesh();

  void SetHeightmap(float *height);
  void SetHeightmapFromFBM(FractalBrownianMotion *fbm);

  inline size_t WidthSteps() const
  {
    return m_widthSteps;
  }

  inline size_t DepthSteps() const
  {
    return m_depthSteps;
  }

protected:
  const size_t m_widthSteps;
  const size_t m_depthSteps;
};
}