/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class TransparentRenderingNode : public SceneNode
{
public:
  TransparentRenderingNode(const std::string &name);
  virtual ~TransparentRenderingNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);
};
}