/** @file */

#include "CubeMapTexture.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new cube map texture.
 */
CubeMapTexture::CubeMapTexture()
    : ITexture(GL_TEXTURE_CUBE_MAP)
{
}

CubeMapTexture::~CubeMapTexture()
{
}

/**
 * @copydoc ITexture::LoadFromFiles
 *
 * Filename order is:
 *  - Positive X (east)
 *  - Negative X (west)
 *  - Positive Y (up)
 *  - Negative Y (down)
 *  - Positive Z (back)
 *  - Negative Z (front)
 */
bool CubeMapTexture::LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags)
{
  if (filenames.size() != 6)
    return false;

  m_textureID =
      SOIL_load_OGL_cubemap(filenames[0].c_str(), filenames[1].c_str(), filenames[2].c_str(), filenames[3].c_str(),
                            filenames[4].c_str(), filenames[5].c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, flags);

  return Valid();
}
}
