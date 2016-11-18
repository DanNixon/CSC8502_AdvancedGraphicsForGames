/** @file */

#include "PositionableCamera.h"

#include <../nclgl/Window.h>

#include "Renderer.h"

namespace GraphicsCoursework
{
PositionableCamera::PositionableCamera(const std::string &name)
    : CameraNode(name)
    , m_linearSpeed(1.0f)
    , m_angularSpeed(1.0f)
    , m_yawAngle(0.0f)
    , m_pitchAngle(0.0f)
{
}

PositionableCamera::~PositionableCamera()
{
}

void PositionableCamera::Update(float msec)
{
  CameraNode::Update(msec);

  if (!m_active)
    return;

  m_pitchAngle += Window::GetMouse()->GetRelativePosition().y * m_angularSpeed;
  m_yawAngle -= Window::GetMouse()->GetRelativePosition().x * m_angularSpeed;

  m_pitchAngle = min(m_pitchAngle, 90.0f);
  m_pitchAngle = max(m_pitchAngle, -90.0f);

  if (m_yawAngle < 0)
    m_yawAngle += 360.0f;

  if (m_yawAngle > 360.0f)
    m_yawAngle -= 360.0f;

  m_localRotation = Matrix4::Rotation(m_yawAngle, Vector3(0.0f, 1.0f, 0.0f));

  float linearDelta = msec * m_linearSpeed;

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_LEFT))
    m_localTransform = m_localTransform * Matrix4::Translation(m_localRotation.GetRightVector() * -linearDelta);

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_RIGHT))
    m_localTransform = m_localTransform * Matrix4::Translation(m_localRotation.GetRightVector() * linearDelta);

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_FORWARDS))
    m_localTransform = m_localTransform * Matrix4::Translation(m_localRotation.GetBackVector() * -linearDelta);

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_BACKWARS))
    m_localTransform = m_localTransform * Matrix4::Translation(m_localRotation.GetBackVector() * linearDelta);

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_UP))
    m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, linearDelta, 0.0f));

  if (Window::GetKeyboard()->KeyDown(KEY_MOVE_DOWN))
    m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, -linearDelta, 0.0f));

  m_localRotation = m_localRotation * Matrix4::Rotation(m_pitchAngle, Vector3(1.0f, 0.0f, 0.0f));
}
}
