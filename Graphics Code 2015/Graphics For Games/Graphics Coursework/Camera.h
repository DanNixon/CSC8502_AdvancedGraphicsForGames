#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class Camera : public SceneNode
{
public:
  Camera(const std::string &name);
  virtual ~Camera();

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

  inline Matrix4 ViewMatrix() const
  {
    return
      Matrix4::Rotation(-m_pitchAngle, Vector3(1, 0, 0)) *
      Matrix4::Rotation(-m_yawAngle, Vector3(0, 1, 0)) *
      Matrix4::Translation(-m_worldTransform.GetPositionVector());
  }

  virtual void Render(RenderState & state);

protected:
  float m_yawAngle;
  float m_pitchAngle;
};
}