#pragma once

#include <GL/glew.h>

// TODO

namespace GraphicsCoursework
{
  class Texture
  {
  public:
    Texture();
    virtual ~Texture();

  protected:
    GLuint m_textureID;
  };
}