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

/**
 * @brief Points the camera towards another scene node.
 * @param thing Scene node to look at
 */
void CameraNode::LookAt(SceneNode * thing)
{
  LookInDirection(m_worldTransform.GetPositionVector() - thing->GetWorldTransformation().GetPositionVector());
}

/**
 * @brief Points the camera in a given direction.
 * @param direction Direction to point camera in
 * @param up Up vector
 */
void CameraNode::LookInDirection(const Vector3 & direction, const Vector3 &up)
{
  Vector3 xaxis = Vector3::Cross(up, direction);
  xaxis.Normalise();

  Vector3 yaxis = Vector3::Cross(direction, xaxis);
  yaxis.Normalise();

  m_localRotation.values[0] = xaxis.x;
  m_localRotation.values[1] = yaxis.x;
  m_localRotation.values[2] = direction.x;
  m_localRotation.values[3] = 0.0f;

  m_localRotation.values[4] = xaxis.y;
  m_localRotation.values[5] = yaxis.y;
  m_localRotation.values[6] = direction.y;
  m_localRotation.values[7] = 0.0f;

  m_localRotation.values[8] = xaxis.z;
  m_localRotation.values[9] = yaxis.z;
  m_localRotation.values[10] = direction.z;
  m_localRotation.values[11] = 0.0f;

  m_localRotation.values[12] = 0.0f;
  m_localRotation.values[13] = 0.0f;
  m_localRotation.values[14] = 0.0f;
  m_localRotation.values[15] = 1.0f;
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
