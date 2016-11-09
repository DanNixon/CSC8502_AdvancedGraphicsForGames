#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class Camera : public SceneNode
{
public:
  Camera();
  virtual ~Camera();

  inline Matrix4 GetProjectionViewMatrix() const
  {
    return m_pvMatrix;
  }

  inline float GetYaw() const
  {
    return m_yawAngle;
  }

  inline void SetYaw(float yaw)
  {
    m_yawAngle = yaw;
    CacheMatrices();
  }

  inline float GetPitch() const
  {
    return m_pitchAngle;
  }

  inline void SetPitch(float pitch)
  {
    m_pitchAngle = pitch;
    CacheMatrices();
  }

  inline Matrix4 GetProjectionMatrix() const
  {
    return m_projectionMatrix;
  }

  inline void SetProjectionMatrix(const Matrix4 &proj)
  {
    m_projectionMatrix = proj;
    CacheMatrices();
  }

protected:
  void CacheMatrices();

protected:
  float m_yawAngle;
  float m_pitchAngle;

  Matrix4 m_projectionMatrix;
  Matrix4 m_pvMatrix;
};
}