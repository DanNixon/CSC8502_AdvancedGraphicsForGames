#pragma once

#include <../nclgl/Matrix4.h>
#include <../nclgl/Plane.h>

namespace GraphicsCoursework
{
class RenderableNode;

/**
 * @class Frustum
 * @brief Represents a view frustum used for out of camera frame clipping.
 */
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
  Plane m_planes[NUM_PLANES]; //!< Planes that make up the frustum
  bool m_passAllTests;        //!< Flag indicating that the intersection test will always pass
};
}
