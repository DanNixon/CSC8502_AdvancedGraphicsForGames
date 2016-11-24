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

  virtual void SetActive(bool active, bool recursive = false);
  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

protected:
  ShaderProgram *m_program;         //!< Shader program to be activated by this node
  ShaderProgram *m_previousProgram; //!< Previous shader activated
};
}
