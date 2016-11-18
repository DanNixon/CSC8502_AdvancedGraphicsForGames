/** @file */

#include "CameraNode.h"

#include "../nclgl/Matrix4.h"

namespace GraphicsCoursework
{
CameraNode::CameraNode(const std::string &name)
    : SceneNode(name)
{
}

CameraNode::~CameraNode()
{
}

Matrix4 CameraNode::ViewMatrix() const
{
  return m_localRotation.GetTransposedRotation() * Matrix4::Translation(-m_worldTransform.GetPositionVector());
}
}
