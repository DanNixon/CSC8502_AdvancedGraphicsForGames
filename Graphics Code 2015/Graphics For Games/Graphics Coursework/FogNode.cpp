/** @file */

#include "FogNode.h"

namespace GraphicsCoursework
{
  FogNode::FogNode(const std::string & name)
    : ShaderDataNode(name)
    , m_colour(0.5f, 0.5, 0.5f, 0.4f)
  {
  }

  FogNode::~FogNode()
  {
  }
}