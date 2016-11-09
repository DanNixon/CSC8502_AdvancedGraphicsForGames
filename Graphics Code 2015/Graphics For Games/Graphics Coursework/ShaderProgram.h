/** @file */

#pragma once

#include <GL/glew.h>
#include <vector>

namespace GraphicsCoursework
{
class Shader;

/**
 * @class ShaderProgram
 * @brief Encapsulation for a GL shader program.
 * @author Dan Nixon
 */
class ShaderProgram
{
public:
  /**
   * @var NUM_SHADERS
   * @brief Maximum number of shaders in a program.
   */
  static const size_t NUM_SHADERS = 5;

  ShaderProgram();
  ShaderProgram(std::vector<Shader *> shaders);
  ~ShaderProgram();

  bool AddShader(Shader *s);

  bool Link();

  /**
   * @brief Gets the GL program.
   * @return GL shader program
   */
  GLuint Program() const
  {
    return m_program;
  }

  /**
   * @brief Tests if the shader program is valid.
   * @return True if valid
   *
   * A program is valid when all shaders are loaded and compiled and the
   * program is linked.
   */
  bool Valid() const
  {
    return m_valid;
  }

private:
  GLuint m_program;               //!< GL shader program
  Shader *m_shaders[NUM_SHADERS]; //!< Array of Shaders in program
  bool m_valid;                   //!< Flag indicating validity of program
};
}