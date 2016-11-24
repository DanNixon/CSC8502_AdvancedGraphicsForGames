/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class CameraNode
 * @brief Scene node containing a basic camera.
 */
class CameraNode : public SceneNode
{
public:
  CameraNode(const std::string &name, bool autoView = true);
  virtual ~CameraNode();

  virtual void LockOrientationTo(SceneNode *node, const Matrix4 &transform = Matrix4());
  virtual void LookAt(SceneNode *thing);
  virtual void LookInDirection(const Vector3 &direction, const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));

  /**
   * @brief Gets the camera's view matrix.
   * @return View matrix
   */
  inline Matrix4 ViewMatrix() const
  {
    return m_viewMatrix;
  }

  virtual void Update(float msec);

protected:
  bool m_autoViewMatrix; //!< Flag indicating that view matrix is computed from position and orientation
  Matrix4 m_viewMatrix;  //!< Cached view matrix

  SceneNode *m_orientationLock;       //!< SceneNode to match orientation of
  Matrix4 m_orientationLockTransform; //!< Transform that is applied after locked orientation is updated
};
}
