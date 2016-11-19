#pragma once

#include <../nclgl/Matrix4.h>
#include <../nclgl/Plane.h>

namespace GraphicsCoursework
{
class RenderableNode;

class Frustum
{
public:
  static const size_t NUM_PLANES;

public:
  Frustum();
  Frustum(const Matrix4 &m);
  virtual ~Frustum();

  void Reset();
  void FromMatrix(const Matrix4 &m);
  bool ContainsSceneNode(RenderableNode *n);

protected:
  Plane m_planes[6];
  bool m_passAllTests;
};
}