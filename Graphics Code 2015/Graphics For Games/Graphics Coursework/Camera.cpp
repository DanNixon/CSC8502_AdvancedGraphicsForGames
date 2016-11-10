#include "Camera.h"

#include "../nclgl/Matrix4.h"

namespace GraphicsCoursework
{
  Camera::Camera(const std::string &name)
    : SceneNode(name)
    , m_yawAngle(0.0f)
    , m_pitchAngle(0.0f)
  {
  }

  Camera::~Camera()
  {
  }

  void Camera::Render(RenderState & state)
  {
    if (m_active)
      state.viewMatrix = ViewMatrix();

    SceneNode::Render(state);

    if (m_active)
      state.viewMatrix.ToIdentity();
  }
}