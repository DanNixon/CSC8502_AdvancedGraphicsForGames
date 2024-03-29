/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>
#include <functional>

namespace GraphicsCoursework
{
/**
 * @class ShaderControlNode
 * @brief Scene node used to inject custom functions into the rendering workflow at shader bind/unbind time when a
 * SHaderSyncNode has been activated.
 */
class ShaderControlNode : public ShaderDataNode
{
public:
  /**
   * @brief Type used for control functions.
   */
  typedef std::function<void(ShaderProgram *)> ControlCallbackFunc;

public:
  ShaderControlNode(const std::string &name, ControlCallbackFunc onBind = [](ShaderProgram *) {},
                    ControlCallbackFunc onUnBind = [](ShaderProgram *) {});
  virtual ~ShaderControlNode();

  /**
   * @brief Gets or sets the function called when binding the shader.
   * @return Reference to function
   */
  inline ControlCallbackFunc &OnBind()
  {
    return m_onBind;
  }

  /**
   * @brief Gets or sets the function called when unbinding the shader.
   * @return Reference to function
   */
  inline ControlCallbackFunc &OnUnBind()
  {
    return m_onUnBind;
  }

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  ControlCallbackFunc m_onBind;   //!< Node entry function
  ControlCallbackFunc m_onUnBind; //!< Node exit function
};
}
