/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
class ProjectionNode : public ShaderDataNode
{
public:
  ProjectionNode(const std::string &name, const Matrix4 &proj);
  virtual ~ProjectionNode();

  inline Matrix4 ProjectionMatrix() const
  {
    return m_projectionMatrix;
  }

  virtual void SetProjectionMatrix(const Matrix4 &proj)
  {
    m_projectionMatrix = proj;
  }

  virtual void ShaderBind(ShaderProgram *s);

protected:
  Matrix4 m_projectionMatrix;
};
}
