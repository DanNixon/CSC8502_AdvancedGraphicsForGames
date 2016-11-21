/** @file */

#include "HeightmapTexture.h"

#include "FractalBrownianMotion.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new heightmap texture.
 * @param width Width in texels
 * @param height Height in texels
 */
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

/**
 * @brief Sets the height values from a configured Fractal Brownian motion generator.
 * @param fbm Reference to FBM generator
 */
void HeightmapTexture::SetFromFBM(FractalBrownianMotion *fbm)
{
  fbm->FractalArrayThreaded(m_yData, m_width, m_height);
  BufferImage(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, m_yData);
}
}
