/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
  class RepeatNode : public SceneNode
  {
  public:
    RepeatNode(const std::string &name, size_t numberOfTimes = 1);
    virtual ~RepeatNode();

    inline size_t &NumberOfTimes()
    {
      return m_numberOfTimes;
    }

    virtual void Render(RenderState &state);

  protected:
    size_t m_numberOfTimes;
  };
}