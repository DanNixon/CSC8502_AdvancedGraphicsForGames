/** @file */

#include "ShaderProgram.h"

#include <iostream>

#include "../nclgl/Buffer.h"

#include "Shader.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new, empty shader program.
 */
ShaderProgram::ShaderProgram()
    : m_valid(false)
{
  for (size_t i = 0; i < NUM_SHADERS; i++)
    m_shaders[i] = nullptr;

  m_program = glCreateProgram();
}

ShaderProgram::ShaderProgram(std::vector<Shader *> shaders)
    : m_valid(false)
{
  size_t i = 0;

  for (auto it = shaders.begin(); it != shaders.end(); ++it)
    m_shaders[i++] = *it;

  for (; i < NUM_SHADERS; i++)
    m_shaders[i] = nullptr;

  m_program = glCreateProgram();
  Link();
}

/**
 * @brief Destroys the shader program, deteching all shaders and deleting the
 *        program.
 */
ShaderProgram::~ShaderProgram()
{
  for (size_t i = 0; i < NUM_SHADERS; i++)
  {
    if (m_shaders[i] != nullptr)
      glDetachShader(m_program, m_shaders[i]->Object());
  }

  glDeleteProgram(m_program);
}

/**
 * @brief Adds a Shader to the program.
 * @param s Shader to add
 * @return True if the shader was added
 */
bool ShaderProgram::AddShader(Shader *s)
{
  for (size_t i = 0; i < NUM_SHADERS; i++)
  {
    if (m_shaders[i] == nullptr)
    {
      m_shaders[i] = s;
      return true;
    }
  }

  return false;
}

/**
 * @brief Links the shader program.
 * @return True if the program was successfully linked
 */
bool ShaderProgram::Link()
{
  if (m_valid)
    return false;

  for (size_t i = 0; i < NUM_SHADERS; i++)
  {
    if (m_shaders[i] != nullptr)
    {
      if (!m_shaders[i]->Valid())
        return false;

      glAttachShader(m_program, m_shaders[i]->Object());
    }
  }

  glBindAttribLocation(m_program, VERTEX_BUFFER, "position");
  glBindAttribLocation(m_program, COLOUR_BUFFER, "colour");
  glBindAttribLocation(m_program, TEXTURE_BUFFER, "texCoord");
  // glBindAttribLocation(m_program, NORMAL_BUFFER, "normal");
  // glBindAttribLocation(m_program, TANGENT_BUFFER, "tangent");

  glLinkProgram(m_program);

  GLint status;
  glGetProgramiv(m_program, GL_LINK_STATUS, &status);

  if (status == GL_FALSE)
  {
    char errorMsg[2048];
    glGetProgramInfoLog(m_program, sizeof(errorMsg), nullptr, errorMsg);
    std::cerr << errorMsg << '\n';
    return false;
  }

  m_valid = (status != GL_FALSE);

  if (!m_valid)
  {
    char errorMsg[2048];
    glGetInfoLogARB(m_program, sizeof(errorMsg), nullptr, errorMsg);
    std::cerr << errorMsg << '\n';
  }

  return m_valid;
}
}