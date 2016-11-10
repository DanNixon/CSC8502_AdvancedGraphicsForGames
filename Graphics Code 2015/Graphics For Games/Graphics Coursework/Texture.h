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
  static void UnBind(GLuint idx);

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

  void BindTo(GLuint target);
  void BindToShader(GLuint program, const std::string &uniformName, GLuint idx);
  void UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx);

protected:
  GLuint m_textureID;
};
}