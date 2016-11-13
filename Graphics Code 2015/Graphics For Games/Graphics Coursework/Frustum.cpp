#include "Frustum.h"

#include "RenderableNode.h"

namespace GraphicsCoursework
{
  Frustum::Frustum()
  {
  }

  Frustum::~Frustum()
  {
  }

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
  }

  bool Frustum::ContainsSceneNode(RenderableNode &n)
  {
    for (int p = 0; p < 6; ++p)
    {
      //if (!m_planes[p].SphereInPlane(n.GetWorldTransformation().GetPositionVector(), n.GetBoundingRadius()))
        return false;
    }

    return true;
  }
}