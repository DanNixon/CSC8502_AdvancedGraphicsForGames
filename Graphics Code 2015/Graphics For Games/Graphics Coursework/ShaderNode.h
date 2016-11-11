/** @file */

#pragma once

#include "SceneNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
class ShaderNode : public SceneNode
{
public:
  ShaderNode(const std::string &name, ShaderProgram *program);
  virtual ~ShaderNode();

  virtual void SetActive(bool active);
  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

private:
  ShaderProgram *m_program;
  GLint m_prevProgram;
};
}
