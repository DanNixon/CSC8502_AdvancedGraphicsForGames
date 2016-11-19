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

  Matrix4 ViewMatrix() const;

  inline Frustum ViewFrustum() const
  {
    return Frustum(ViewMatrix());
  }
};
}
