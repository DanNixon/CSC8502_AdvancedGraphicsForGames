/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class CameraNode
 * @brief Scenen node containing a basic camera.
 */
class CameraNode : public SceneNode
{
public:
  CameraNode(const std::string &name);
  virtual ~CameraNode();

  virtual void LockOrientationTo(SceneNode *node);

  Matrix4 ViewMatrix() const;

  virtual void Update(float msec);

protected:
  SceneNode *m_orientationLock;
};
}
