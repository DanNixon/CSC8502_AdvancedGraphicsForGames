/** @file */

#include "CubeMapTexture.h"

namespace GraphicsCoursework
{
CubeMapTexture::CubeMapTexture()
{
}

CubeMapTexture::~CubeMapTexture()
{
}

bool CubeMapTexture::LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags)
{
  if (filenames.size() != 6)
    return false;

  m_textureID = SOIL_load_OGL_cubemap(
      filenames[0].c_str(), filenames[1].c_str(), filenames[2].c_str(), filenames[3].c_str(),
      filenames[4].c_str(), filenames[5].c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);

  return Valid();
}
}