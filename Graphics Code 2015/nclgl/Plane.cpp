#include "Plane.h"

Plane::Plane(const Vector3 &normal, float distance, bool normalise)
{
  if (normalise)
  {
    float length = sqrt(Vector3::Dot(normal, normal));

    m_normal = normal / length;
    m_distance = distance / length;
  }
  else
  {
    m_normal = normal;
    m_distance = distance;
  }
}

bool Plane::SphereInPlane(const Vector3 &position, float radius) const
{
  return (Vector3::Dot(position, m_normal) + m_distance <= -radius);
}
