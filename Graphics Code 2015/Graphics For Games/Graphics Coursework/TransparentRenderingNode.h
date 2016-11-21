/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class TransparentRenderingNode
 * @brief Used to render all transparent nodes in the scene graph under this node.
 *
 * Allows finer control of the time at which transparent nodes are rendered.
 */
class TransparentRenderingNode : public SceneNode
{
public:
  TransparentRenderingNode(const std::string &name);
  virtual ~TransparentRenderingNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);
};
}
