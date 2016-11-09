#include "Camera.h"

namespace GraphicsCoursework
{
  Camera::Camera()
    : SceneNode("camera")
    , m_yawAngle(0.0f)
    , m_pitchAngle(0.0f)
  {
    m_projectionMatrix.ToIdentity();
    CacheMatrices();
  }

  Camera::~Camera()
  {
  }

  void Camera::CacheMatrices()
  {
    // TODO
  }
}