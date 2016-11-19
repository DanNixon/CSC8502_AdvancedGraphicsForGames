/** @file */

#include "HeightmapTexture.h"

#include "FractalBrownianMotion.h"

namespace GraphicsCoursework
{
HeightmapTexture::HeightmapTexture(GLsizei width, GLsizei height)
    : GeneratedTexture(width, height)
    , m_yData(new float[width * height])
{
  memset(m_yData, 0, width * height);
  BufferImage(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
}

HeightmapTexture::~HeightmapTexture()
{
  delete[] m_yData;
}

void HeightmapTexture::SetFromFBM(FractalBrownianMotion * fbm)
{
  fbm->FractalArrayThreaded(m_yData, m_width, m_height);
  BufferImage(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, m_yData);
}
}