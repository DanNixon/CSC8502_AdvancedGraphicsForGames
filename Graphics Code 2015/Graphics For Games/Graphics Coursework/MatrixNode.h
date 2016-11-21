/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
/**
 * @class MatrixNode
 * @brief Scene node used to set a mat4 uniform in the active shader.
 *
 * Useful for projection, texture, view, etc. matrices.
 */
class MatrixNode : public ShaderDataNode
{
public:
  MatrixNode(const std::string &name, const std::string &uniformName, const Matrix4 &matrix = Matrix4());
  virtual ~MatrixNode();

  /**
   * @brief Gets or sets the matrix that will be assigned to the uniform.
   * @return Reference to matrix
   */
  inline Matrix4 &Matrix()
  {
    return m_matrix;
  }

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  const std::string m_uniformName; //!< Name of the uniform the matrix will be assigned to
  Matrix4 m_matrix;                //!< Matrix values
};
}
