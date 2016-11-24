/** @file */

#include "CameraNode.h"

#include "../nclgl/Matrix4.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new generic camera.
 * @param name Node name
 * @param autoView If the view matrix should be computed automatically form position and orientation
 */
CameraNode::CameraNode(const std::string &name, bool autoView)
    : SceneNode(name)
    , m_autoViewMatrix(autoView)
    , m_orientationLock(nullptr)
{
  m_viewMatrix.ToIdentity();
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

/**
 * @brief Points the camera towards another scene node.
 * @param thing Scene node to look at
 */
void CameraNode::LookAt(SceneNode *thing)
{
  m_viewMatrix = Matrix4::BuildViewMatrix(m_worldTransform.GetPositionVector(),
                                          thing->GetWorldTransformation().GetPositionVector());
}

/**
 * @brief Points the camera in a given direction.
 * @param direction Direction to point camera in
 * @param up Up vector
 */
void CameraNode::LookInDirection(const Vector3 &direction, const Vector3 &up)
{
  m_viewMatrix = Matrix4::BuildViewMatrix(m_worldTransform.GetPositionVector(),
                                          m_worldTransform.GetPositionVector() + direction, up);
}

/**
 * @copydoc SceneNode::Update
 */
void CameraNode::Update(float msec)
{
  SceneNode::Update(msec);

  if (m_orientationLock != nullptr)
    m_localRotation = m_orientationLock->GetLocalRotation() * m_orientationLockTransform;

  if (m_autoViewMatrix)
    m_viewMatrix =
        m_localRotation.GetTransposedRotation() * Matrix4::Translation(-m_worldTransform.GetPositionVector());
}
}
