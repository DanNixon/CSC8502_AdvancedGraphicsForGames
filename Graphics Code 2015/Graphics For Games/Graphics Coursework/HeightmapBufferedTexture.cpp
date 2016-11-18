/** @file */

#include "HeightmapBufferedTexture.h"

#include "FractalBrownianMotion.h"

namespace GraphicsCoursework
{
HeightmapBufferedTexture::HeightmapBufferedTexture(GLsizei width, GLsizei height)
    : BufferedTexture(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE)
{
}

HeightmapBufferedTexture::~HeightmapBufferedTexture()
{
}

void HeightmapBufferedTexture::SetFromFBM(FractalBrownianMotion * fbm)
{
  // TODO
}
}