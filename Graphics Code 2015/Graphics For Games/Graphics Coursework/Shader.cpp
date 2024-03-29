/** @file */

#include "Shader.h"

#include <fstream>
#include <iostream>

namespace GraphicsCoursework
{
/**
 * @brief Creates a new shader.
 * @param filename GLSL source file
 * @param stage Shader stage
 */
Shader::Shader(std::string filename, GLuint stage)
    : m_valid(false)
    , m_stage(stage)
{
  m_valid = Compile(filename);
}

/**
 * @brief Deletes the programs associated with the shader.
 */
Shader::~Shader(void)
{
  glDeleteShader(m_shaderObject);
}

/**
 * @brief Loads the contents of a file into a string.
 * @param filename Name of file to load
 * @param into [out] String to store contents in
 * @return True on success, false if file is not found
 */
bool Shader::LoadFile(std::string filename, std::string &into)
{
  std::ifstream file;

  file.open(filename);
  if (!file)
  {
    std::cerr << "Shader source file \"" << filename << "\" not found!\n";
    return false;
  }

  file.seekg(0, std::ios::end);
  into.resize(1 + (unsigned int)file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(&into[0], into.size());

  into[into.size() - 1] = '\n';

  file.close();
  return true;
}

/**
 * @brief Compiles a shader.
 * @param filename GLSL source file
 * @return True for successful compilation
 */
bool Shader::Compile(std::string filename)
{
  std::string load;
  if (!LoadFile(filename, load))
    return false;

  m_shaderObject = glCreateShader(m_stage);

  const char *chars = load.c_str();
  glShaderSource(m_shaderObject, 1, &chars, nullptr);
  glCompileShader(m_shaderObject);

  GLint status;
  glGetShaderiv(m_shaderObject, GL_COMPILE_STATUS, &status);
  bool success = (status != GL_FALSE);

  if (!success)
  {
    char errorMsg[2048];
    glGetInfoLogARB(m_shaderObject, sizeof(errorMsg), nullptr, errorMsg);
    std::cerr << errorMsg << '\n';
  }

  return success;
}
}