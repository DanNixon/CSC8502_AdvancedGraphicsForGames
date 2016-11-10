/** @file */

#pragma once

#include <GL/glew.h>
#include <SOIL.h>
#include <string>

namespace GraphicsCoursework
{
  class Texture
  {
  public:
    static void UnBind(GLuint target);

  public:
    Texture();
    virtual ~Texture();

    bool LoadFromFile(const std::string &filename);
    
    inline bool Valid() const
    {
      return m_textureID != 0;
    }

    inline GLuint GetTextureID()
    {
      return m_textureID;
    }

    void BindTo(GLint target);
    void BindToShader(GLuint program, const std::string &uniformName, GLint idx);

  protected:
    GLuint m_textureID;
  };
}