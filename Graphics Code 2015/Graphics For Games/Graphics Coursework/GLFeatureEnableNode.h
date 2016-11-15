/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
class GLFeatureEnableNode : public ShaderDataNode
{
public:
  GLFeatureEnableNode(const std::string &name, GLenum feature, GLboolean setState);
  virtual ~GLFeatureEnableNode();

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  GLenum m_featureEnum;
  GLboolean m_setState;
  GLboolean m_previousState;
};
}
