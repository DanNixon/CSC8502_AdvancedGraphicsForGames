/** @file */

#include "Frustum.h"

#include "RenderableNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new empty frustum.
 */
Frustum::Frustum()
    : m_passAllTests(true)
{
}

/**
 * @brief Create a new frustum from a view-projection matrix.
 * @param m VP matrix
 */
Frustum::Frustum(const Matrix4 &m)
    : m_passAllTests(false)
{
  FromMatrix(m);
}

Frustum::~Frustum()
{
}

/**
 * @brief Resets the frustum to one of zero volume.
 */
void Frustum::Reset()
{
  for (int i = 0; i < NUM_PLANES; i++)
    m_planes[i].Reset();

  m_passAllTests = true;
}

/**
 * @brief Builds the frustum from a view-projection matrix.
 * @param m VP matrix
 */
void Frustum::FromMatrix(const Matrix4 &m)
{
  Vector3 xAxis = Vector3(m.values[0], m.values[4], m.values[8]);
  Vector3 yAxis = Vector3(m.values[1], m.values[5], m.values[9]);
  Vector3 zAxis = Vector3(m.values[2], m.values[6], m.values[10]);
  Vector3 wAxis = Vector3(m.values[3], m.values[7], m.values[11]);

  // RIGHT
  m_planes[0] = Plane(wAxis - xAxis, (m.values[15] - m.values[12]), true);
  // LEFT
  m_planes[1] = Plane(wAxis + xAxis, (m.values[15] + m.values[12]), true);
  // BOTTOM
  m_planes[2] = Plane(wAxis + yAxis, (m.values[15] + m.values[13]), true);
  // TOP
  m_planes[3] = Plane(wAxis - yAxis, (m.values[15] - m.values[13]), true);
  // FAR
  m_planes[4] = Plane(wAxis - zAxis, (m.values[15] - m.values[14]), true);
  // NEAR
  m_planes[5] = Plane(wAxis + zAxis, (m.values[15] + m.values[14]), true);

  m_passAllTests = false;
}

/**
 * @brief Tests if a renderable node may be contained in this frustum.
 * @param n Node to test
 * @return True if the node (partially or fully) overlaps with the frustum
 */
bool Frustum::ContainsSceneNode(RenderableNode *n)
{
  bool retVal = true;

  if (!m_passAllTests)
  {
    for (int i = 0; i < NUM_PLANES; i++)
    {
      if (!m_planes[i].SphereInPlane(n->GetWorldTransformation().GetPositionVector(), n->BoundingSphereRadius()))
      {
        retVal = false;
        break;
      }
    }
  }

  return retVal;
}
}
