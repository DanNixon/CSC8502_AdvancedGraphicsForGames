/** @file */

#include "MatrixNode.h"

#include "GL/glew.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new matrix node.
 * @param name Node name
 * @param uniformName Shader uniform name
 * @param matrix Initial value of matrix (defaults to identity)
 */
MatrixNode::MatrixNode(const std::string &name, const std::string &uniformName, const Matrix4 &matrix)
    : ShaderDataNode(name)
    , m_uniformName(uniformName)
    , m_matrix(matrix)
{
}

MatrixNode::~MatrixNode()
{
}

/**
 * @copydoc ShaderDataNode::ShaderBind
 */
void MatrixNode::ShaderBind(ShaderProgram *s)
{
  glUniformMatrix4fv(glGetUniformLocation(s->Program(), m_uniformName.c_str()), 1, false, (float *)&m_matrix);
}

/**
 * @copydoc ShaderDataNode::ShaderUnBind
 */
void MatrixNode::ShaderUnBind(ShaderProgram *s)
{
  Matrix4 mat;
  mat.ToIdentity();

  glUniformMatrix4fv(glGetUniformLocation(s->Program(), m_uniformName.c_str()), 1, false, (float *)&mat);
}
}
