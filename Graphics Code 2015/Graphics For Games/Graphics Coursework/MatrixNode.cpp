/** @file */

#include "MatrixNode.h"

#include "GL/glew.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
MatrixNode::MatrixNode(const std::string &name, const std::string &uniformName, const Matrix4 &proj)
    : ShaderDataNode(name)
    , m_uniformName(uniformName)
    , m_matrix(proj)
{
}

MatrixNode::~MatrixNode()
{
}

void MatrixNode::ShaderBind(ShaderProgram *s)
{
  glUniformMatrix4fv(glGetUniformLocation(s->Program(), m_uniformName.c_str()), 1, false,
                     (float *)&m_matrix);
}

void MatrixNode::ShaderUnBind(ShaderProgram *s)
{
  Matrix4 mat;
  mat.ToIdentity();

  glUniformMatrix4fv(glGetUniformLocation(s->Program(), m_uniformName.c_str()), 1, false,
                     (float *)&mat);
}
}
