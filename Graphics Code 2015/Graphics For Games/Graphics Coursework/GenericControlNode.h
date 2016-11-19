/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Matrix4.h>
#include <functional>

namespace GraphicsCoursework
{
/**
 * @class GenericControlNode
 * @brief Scene node used to inject custom functions into the rendering workflow.
 */
class GenericControlNode : public ShaderDataNode
{
public:
  /**
   * @brief Type used for control functions.
   */
  typedef std::function<void(ShaderProgram *)> ControlCallbackFunc;

public:
  GenericControlNode(const std::string &name, ControlCallbackFunc onBind =
                                                  [](ShaderProgram *)
                                              {
                                              },
                     ControlCallbackFunc onUnBind =
                         [](ShaderProgram *)
                     {
                     });
  virtual ~GenericControlNode();

  /**
   * @brief Gets or sets the function called when entering this node.
   * @return Reference to function
   */
  inline ControlCallbackFunc &OnBind()
  {
    return m_onBind;
  }

  /**
   * @brief Gets or sets the function called when leaving this node.
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
