/** @file */

#include "Texture.h"

namespace GraphicsCoursework
{
void Texture::UnBind(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
    : m_textureID(0)
{
}

Texture::~Texture()
{
  glDeleteTextures(1, &m_textureID);
}

bool Texture::LoadFromFile(const std::string &filename, unsigned int flags)
{
  m_textureID = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
  return Valid();
}

void Texture::BindTo(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::BindToShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  BindTo(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), idx);
}

void Texture::UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  UnBind(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), 0);
}
}