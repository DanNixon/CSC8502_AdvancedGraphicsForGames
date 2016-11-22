/** @file */

#include "CameraNode.h"

#include "../nclgl/Matrix4.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new generic camera.
 * @param name Node name
 */
CameraNode::CameraNode(const std::string &name)
    : SceneNode(name)
    , m_orientationLock(nullptr)
{
}

CameraNode::~CameraNode()
{
}

/**
 * @brief Locks this camera to the same orientation as another node.
 * @param node Node to match
 *
 * Position of this camera is updated on calling Update().
 */
void CameraNode::LockOrientationTo(SceneNode *node, const Matrix4 &transform)
{
  m_orientationLock = node;
  m_orientationLockTransform = transform;
}

void CameraNode::LookAt(SceneNode * thing)
{
  LookInDirection(m_worldTransform.GetPositionVector() - thing->GetWorldTransformation().GetPositionVector());
}

void CameraNode::LookInDirection(const Vector3 & direction)
{
  // TODO
}

/**
 * @brief Gets the camera's view matrix.
 * @return View matrix
 */
Matrix4 CameraNode::ViewMatrix() const
{
  return m_localRotation.GetTransposedRotation() * Matrix4::Translation(-m_worldTransform.GetPositionVector());
}

/**
 * @copydoc SceneNode::Update
 */
void CameraNode::Update(float msec)
{
  SceneNode::Update(msec);

  if (m_orientationLock != nullptr)
    m_localRotation = m_orientationLock->GetLocalRotation() * m_orientationLockTransform;
}
}
