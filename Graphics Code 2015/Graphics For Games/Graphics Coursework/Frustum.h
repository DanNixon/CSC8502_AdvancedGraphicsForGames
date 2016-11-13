#pragma once

#include <../nclgl/Matrix4.h>
#include <../nclgl/Plane.h>

namespace GraphicsCoursework
{
class RenderableNode;

class Frustum
{
public:
  Frustum();
  virtual ~Frustum();

  void FromMatrix(const Matrix4 &m);
  bool ContainsSceneNode(RenderableNode &n);

protected:
  Plane m_planes[6];
};
}