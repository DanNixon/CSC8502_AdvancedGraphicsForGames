/** @file */

#pragma once

#include "MeshNode.h"

namespace GraphicsCoursework
{
  class Font;

  class TextNode : public MeshNode
  {
  public:
    TextNode(const std::string & name, Font * font, size_t len = 32);
    virtual ~TextNode();

    void SetText(const std::string &text);
  };
}