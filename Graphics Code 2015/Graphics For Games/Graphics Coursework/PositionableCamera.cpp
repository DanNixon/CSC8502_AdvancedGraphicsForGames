/** @file */

#include "PositionableCamera.h"

#include <../nclgl/Window.h>

#include "Renderer.h"

namespace GraphicsCoursework
{
  PositionableCamera::PositionableCamera(const std::string &name)
    : CameraNode(name)
    , m_speed(1.0f)
  {
  }

  PositionableCamera::~PositionableCamera()
  {
  }

  void PositionableCamera::Update(float msec)
  {
    SceneNode::Update(msec);

    if (!m_active)
      return;

    // TODO: mouse should give delta angle

    Vector2 normalisedMousePos = Window::GetMouse()->GetAbsolutePosition() / m_renderer->ParentWindow().GetScreenSize();
    std::cout << normalisedMousePos << '\n';

    m_pitchAngle -= Window::GetMouse()->GetRelativePosition().y;
    m_yawAngle -= Window::GetMouse()->GetRelativePosition().x;

    m_pitchAngle = min(m_pitchAngle, 90.0f);
    m_pitchAngle = max(m_pitchAngle, -90.0f);

    if (m_yawAngle < 0)
      m_yawAngle += 360.0f;

    if (m_yawAngle > 360.0f)
      m_yawAngle -= 360.0f;

    msec *= m_speed;

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_LEFT))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(-msec, 0.0f, 0.0f));

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_RIGHT))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(msec, 0.0f, 0.0f));

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_FORWARDS))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, 0.0f, -msec));

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_BACKWARS))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, 0.0f, msec));

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_UP))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, msec, 0.0f));

    if (Window::GetKeyboard()->KeyDown(KEY_MOVE_DOWN))
      m_localTransform = m_localTransform * Matrix4::Translation(Vector3(0.0f, -msec, 0.0f));
  }
}
