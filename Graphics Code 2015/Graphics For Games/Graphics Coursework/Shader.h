/** @file */

#pragma once

#include <string>

#include "GL/glew.h"

namespace GraphicsCoursework
{
/**
 * @class Shader
 * @brief Encapsulation for a single OpenGL shader.
 * @author Dan Nixon
 */
class Shader
{
public:
  Shader(std::string filename, GLuint stage);
  virtual ~Shader();

  /**
   * @brief Returns the type of shader (shader stage).
   * @return Shader stage
   */
  GLuint Stage() const
  {
    return m_stage;
  }

  /**
   * @brief Returns the GL shader object.
   * @return Shader object
   */
  GLuint Object() const
  {
    return m_shaderObject;
  }

  /**
   * @brief Checks if the shader has been compiled.
   * @return True if shader is valid
   */
  bool Valid() const
  {
    return m_valid;
  }

private:
  bool LoadFile(std::string filename, std::string &into);
  bool Compile(std::string filename);

private:
  GLuint m_stage;        //!< Shader stage
  GLuint m_shaderObject; //!< GL shader object

  bool m_valid; //!< Flag indicating validity of shader
};
}