/** @file */

#pragma once

#include <GL/glew.h>
#include <SOIL.h>
#include <vector>

namespace GraphicsCoursework
{
/**
 * @class ITexture
 * @brief Class containing basic data for a GL texture.
 */
class ITexture
{
public:
  ITexture(GLuint type);
  virtual ~ITexture();

  virtual bool LoadFromFile(const std::string &filename, unsigned int flags = 0);
  virtual bool LoadFromFiles(const std::vector<std::string> &filenames, unsigned int flags = 0);

  /**
   * @brief Tests if this texture is a valid GL texture.
   * @return True if this container is associated with a GL texture
   */
  inline bool Valid() const
  {
    return m_textureID != 0;
  }

  /**
   * @brief Returns the texture type.
   * @return Texture type
   */
  inline GLuint Type() const
  {
    return m_type;
  }

  /**
   * @brief Returns the GL texture ID.
   * @return Texture number
   */
  inline GLuint GetTextureID()
  {
    return m_textureID;
  }

  virtual void SetRepeating(bool repeat);

  void BindTo(GLuint idx);
  void UnBind(GLuint idx);

  void BindToShader(GLuint program, const std::string &uniformName, GLuint idx);
  void UnBindFromShader(GLuint program, const std::string &uniformName, GLuint idx);

protected:
  GLuint m_type;      //!< OpenGL texture type
  GLuint m_textureID; //!< OpenGL texture handle
};
}
