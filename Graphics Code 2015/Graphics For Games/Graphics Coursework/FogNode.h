/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
  class FogNode : public ShaderDataNode
  {
  public:
    FogNode(const std::string &name);
    virtual ~FogNode();

  protected:
    Vector4 m_colour;
  };
}