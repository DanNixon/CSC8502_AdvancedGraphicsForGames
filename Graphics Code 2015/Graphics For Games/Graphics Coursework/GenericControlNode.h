/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <functional>
#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
class GenericControlNode : public ShaderDataNode
{
public:
  typedef std::function<void(ShaderProgram *)> ControlCallbackFunc;

public:
  GenericControlNode(const std::string &name);
  virtual ~GenericControlNode();

  inline ControlCallbackFunc &OnBind()
  {
    return m_onBind;
  }

  inline ControlCallbackFunc &OnUnBind()
  {
    return m_onUnBind;
  }

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  ControlCallbackFunc m_onBind;
  ControlCallbackFunc m_onUnBind;
};
}
