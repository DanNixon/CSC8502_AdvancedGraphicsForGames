/** @file */

#include "ITexture.h"

namespace GraphicsCoursework
{
void ITexture::UnBind(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(GL_TEXTURE_2D, 0);
}

ITexture::ITexture()
    : m_textureID(0)
{
}

ITexture::~ITexture()
{
  glDeleteTextures(1, &m_textureID);
}

void ITexture::BindTo(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void ITexture::BindToShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  BindTo(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), idx);
}

void ITexture::UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  UnBind(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), 0);
}
}