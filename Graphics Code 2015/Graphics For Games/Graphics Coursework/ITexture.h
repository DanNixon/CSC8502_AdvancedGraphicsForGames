/** @file */

#pragma once

#include <GL/glew.h>
#include <SOIL.h>
#include <vector>

namespace GraphicsCoursework
{
class ITexture
{
public:
  static void UnBind(GLuint idx);

public:
  ITexture();
  virtual ~ITexture();

  virtual bool LoadFromFile(const std::string &filename, unsigned int flags = 0);
  virtual bool LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags = 0);

  inline bool Valid() const
  {
    return m_textureID != 0;
  }

  inline GLuint GetTextureID()
  {
    return m_textureID;
  }

  void BindTo(GLuint target);
  void BindToShader(GLuint program, const std::string &uniformName, GLuint idx);
  void UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx);

protected:
  GLuint m_textureID;
};
}