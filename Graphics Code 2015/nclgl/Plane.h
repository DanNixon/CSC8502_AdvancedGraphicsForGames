#pragma once

#include "Vector3.h"

class Plane
{
public:
  Plane(){};
  Plane(const Vector3 &normal, float distance, bool normalise = false);
  ~Plane(){};

  inline void SetNormal(const Vector3 &normal)
  {
    m_normal = normal;
  }

  inline Vector3 GetNormal() const
  {
    return m_normal;
  }

  inline void SetDistance(float distance)
  {
    m_distance = distance;
  }

  inline float GetDistance() const
  {
    return m_distance;
  }

  bool SphereInPlane(const Vector3 &position, float radius) const;

protected:
  Vector3 m_normal;
  float m_distance;
};
