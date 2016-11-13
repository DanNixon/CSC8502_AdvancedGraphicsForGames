/** @file */

#pragma once

#include "SceneNode.h"

#include "Frustum.h"

namespace GraphicsCoursework
{
class CameraNode : public SceneNode
{
public:
  CameraNode(const std::string &name);
  virtual ~CameraNode();

  inline float GetYaw() const
  {
    return m_yawAngle;
  }

  inline void SetYaw(float yaw)
  {
    m_yawAngle = yaw;
  }

  inline float GetPitch() const
  {
    return m_pitchAngle;
  }

  inline void SetPitch(float pitch)
  {
    m_pitchAngle = pitch;
  }

  virtual void UpdateCaches();

  inline Matrix4 ViewMatrix() const
  {
    return m_viewMatrix;
  }

  inline Frustum Frustum() const
  {
    return m_viewFrustum;
  }

protected:
  float m_yawAngle;
  float m_pitchAngle;

  Matrix4 m_viewMatrix;
  Frustum m_viewFrustum;
};
}
