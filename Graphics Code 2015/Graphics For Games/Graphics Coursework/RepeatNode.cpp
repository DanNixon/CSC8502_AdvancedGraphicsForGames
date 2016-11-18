/** @file */

#include "RepeatNode.h"

namespace GraphicsCoursework
{
  RepeatNode::RepeatNode(const std::string & name, size_t numberOfTimes)
    : SceneNode(name)
    , m_numberOfTimes(numberOfTimes)
  {
  }

  RepeatNode::~RepeatNode()
  {
  }

  void RepeatNode::Render(RenderState & state)
  {
    if (m_active)
    {
      for (size_t i = 0; i < m_numberOfTimes; i++)
        SceneNode::Render(state);
    }
  }
}