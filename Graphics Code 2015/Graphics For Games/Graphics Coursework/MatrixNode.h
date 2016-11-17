/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
class MatrixNode : public ShaderDataNode
{
public:
  MatrixNode(const std::string &name, const std::string &uniformName,
             const Matrix4 &proj = Matrix4());
  virtual ~MatrixNode();

  inline Matrix4 &Matrix()
  {
    return m_matrix;
  }

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  const std::string m_uniformName;
  Matrix4 m_matrix;
};
}
