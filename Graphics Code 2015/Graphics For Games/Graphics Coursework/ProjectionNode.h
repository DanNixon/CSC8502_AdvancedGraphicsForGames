#pragma once

#include "SceneNode.h"

#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
class ProjectionNode : public SceneNode
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

  virtual void Render(RenderState & state);

protected:
  Matrix4 m_projectionMatrix;
};
}