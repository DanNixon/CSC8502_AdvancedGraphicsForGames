/** @file */

#include "ITexture.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new texture of a given type.
 * @param type GL texture type
 */
ITexture::ITexture(GLuint type)
    : m_textureID(0)
    , m_type(type)
{
}

ITexture::~ITexture()
{
  glDeleteTextures(1, &m_textureID);
}

/**
 * @brief Loads a texture from a file.
 * @param filename File to load
 * @param flags Loading flags passed to loader
 * @return True if a valid GL texture was loaded
 */
bool ITexture::LoadFromFile(const std::string &filename, unsigned int flags)
{
  return false;
}

/**
 * @brief Loads a texture from a collection of files.
 * @param filenames Vector of files to load
 * @param flags Loading flags passed to loader
 * @return True if a valid GL texture was loaded
 */
bool ITexture::LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags)
{
  return false;
}

/**
 * @brief Sets the repeating flags for this texture.
 * @param repeat Boolean indicating if texture should be repeating
 */
void ITexture::SetRepeating(bool repeat)
{
  glBindTexture(m_type, m_textureID);
  glTexParameteri(m_type, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP);
  glTexParameteri(m_type, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP);
  glBindTexture(m_type, 0);
}

/**
 * @brief Binds the underlaying GL texture to a given texture index.
 * @param idx Texture index
 */
void ITexture::BindTo(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(m_type, m_textureID);
}

/**
 * @brief Unbinds the underlaying GL texture from a given texture index.
 * @param idx Texture index
 */
void ITexture::UnBind(GLuint idx)
{
  glActiveTexture(GL_TEXTURE0 + idx);
  glBindTexture(m_type, 0);
}

/**
 * @brief Binds the underlaying GL texture to a sampler uniform in a given shader.
 * @param program GL shader program handle
 * @param uniformName Uniform name
 * @param idx Texture index
 */
void ITexture::BindToShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  BindTo(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), idx);
}

/**
 * @brief Unbinds the underlaying GL texture from a sampler uniform in a given shader.
 * @param program GL shader program handle
 * @param uniformName Uniform name
 * @param idx Texture index
 */
void ITexture::UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx)
{
  UnBind(idx);
  glUniform1i(glGetUniformLocation(program, uniformName.c_str()), 0);
}
}
