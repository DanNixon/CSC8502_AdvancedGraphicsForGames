/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class ShaderDataNode : public SceneNode
{
public:
  ShaderDataNode(const std::string &name);
  virtual ~ShaderDataNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s)
  {
  }

  virtual void ShaderUnBind(ShaderProgram *s)
  {
  }
};
};