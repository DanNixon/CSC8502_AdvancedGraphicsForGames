/** @file */

#pragma once

#include "SceneNode.h"

#include <../nclgl/Matrix4.h>
#include <functional>

namespace GraphicsCoursework
{
class GenericControlNodeSingle : public SceneNode
{
public:
  /**
   * @brief Type used for control functions.
   */
  typedef std::function<void(RenderState &state)> ControlCallbackFunc;

public:
  GenericControlNodeSingle(const std::string &name, ControlCallbackFunc onBind = [](RenderState &) {},
                           ControlCallbackFunc onUnBind = [](RenderState &) {});
  virtual ~GenericControlNodeSingle();

  inline ControlCallbackFunc &OnEntry()
  {
    return m_onEntry;
  }

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
