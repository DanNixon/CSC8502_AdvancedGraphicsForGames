/** @file */

#pragma once

#include "SceneNode.h"

#include "Frustum.h"

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
};
}
