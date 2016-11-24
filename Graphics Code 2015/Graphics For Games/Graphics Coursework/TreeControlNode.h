/** @file */

#pragma once

#include "SceneNode.h"

#include <../nclgl/Matrix4.h>
#include <functional>

namespace GraphicsCoursework
{
class TreeControlNode : public SceneNode
{
public:
  /**
   * @brief Type used for control functions.
   */
  typedef std::function<void(RenderState &state)> ControlCallbackFunc;

public:
  TreeControlNode(const std::string &name, ControlCallbackFunc onBind = [](RenderState &) {},
                  ControlCallbackFunc onUnBind = [](RenderState &) {});
  virtual ~TreeControlNode();

  /**
   * @brief Gets or sets the function called when entering this node.
   * @return Reference to function
   */
  inline ControlCallbackFunc &OnEntry()
  {
    return m_onEntry;
  }

  /**
   * @brief Gets or sets the function called when exiting this node.
   * @return Reference to function
   */
  inline ControlCallbackFunc &OnExit()
  {
    return m_onExit;
  }

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

protected:
  ControlCallbackFunc m_onEntry; //!< Node entry function
  ControlCallbackFunc m_onExit;  //!< Node exit function
};
}
