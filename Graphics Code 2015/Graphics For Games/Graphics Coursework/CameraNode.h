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
  CameraNode(const std::string &name);
  virtual ~CameraNode();

  virtual void LockOrientationTo(SceneNode *node, const Matrix4 &transform = Matrix4());
  virtual void LookAt(SceneNode * thing);
  virtual void LookInDirection(const Vector3& direction);

  Matrix4 ViewMatrix() const;

  virtual void Update(float msec);

protected:
  SceneNode *m_orientationLock;       //!< SceneNode to match orientation of
  Matrix4 m_orientationLockTransform; //!< Transform that is applied after locked orientation is updated
};
}
