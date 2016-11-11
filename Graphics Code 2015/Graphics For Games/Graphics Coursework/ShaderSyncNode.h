/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class ShaderSyncNode : public SceneNode
{
public:
  ShaderSyncNode(const std::string &name);
  virtual ~ShaderSyncNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);
};
}