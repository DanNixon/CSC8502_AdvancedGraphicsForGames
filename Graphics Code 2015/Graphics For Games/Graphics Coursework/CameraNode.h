/** @file */

#pragma once

#include "SceneNode.h"

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

  Matrix4 ViewMatrix() const;

protected:
  float m_yawAngle;
  float m_pitchAngle;
};
}
