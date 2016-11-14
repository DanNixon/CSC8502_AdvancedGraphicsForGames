/** @file */

#include "CameraNode.h"

#include "../nclgl/Matrix4.h"

namespace GraphicsCoursework
{
CameraNode::CameraNode(const std::string &name)
    : SceneNode(name)
    , m_yawAngle(0.0f)
    , m_pitchAngle(0.0f)
{
}

CameraNode::~CameraNode()
{
}

Matrix4 CameraNode::ViewMatrix() const
{
  return m_localRotation * Matrix4::Translation(-m_worldTransform.GetPositionVector());
}

void CameraNode::Update(float msec)
{
  SceneNode::Update(msec);

  m_localRotation = Matrix4::Rotation(-m_pitchAngle, Vector3(1, 0, 0)) *
                    Matrix4::Rotation(-m_yawAngle, Vector3(0, 1, 0));
}
}
