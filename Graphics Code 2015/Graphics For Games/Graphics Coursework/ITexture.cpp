/** @file */

#include "ITexture.h"

namespace GraphicsCoursework
{
ITexture::ITexture(GLuint type)
    : m_textureID(0)
    , m_type(type)
{
}

ITexture::~ITexture()
{
  glDeleteTextures(1, &m_textureID);
}

bool ITexture::LoadFromFile(const std::string &filename, unsigned int flags)
{
  return false;
}

bool ITexture::LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags)
{
  return false;
}

void ITexture::SetRepeating(bool repeat)
{
  glBindTexture(m_type, m_textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP);
  glBindTexture(m_type, 0);
}

void ITexture::BindTo(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(m_type, m_textureID);
}

void ITexture::UnBind(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(m_type, 0);
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