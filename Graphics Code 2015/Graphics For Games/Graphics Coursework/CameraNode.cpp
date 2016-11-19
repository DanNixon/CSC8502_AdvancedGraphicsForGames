/** @file */

#include "CameraNode.h"

#include "../nclgl/Matrix4.h"

#include "Frustum.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new generic camera.
 * @param name Node name
 */
CameraNode::CameraNode(const std::string &name)
    : SceneNode(name)
{
}

CameraNode::~CameraNode()
{
}

/**
 * @brief Gets the camera's view matrix.
 * @return View matrix
 */
Matrix4 CameraNode::ViewMatrix() const
{
  return m_localRotation.GetTransposedRotation() * Matrix4::Translation(-m_worldTransform.GetPositionVector());
}
}
