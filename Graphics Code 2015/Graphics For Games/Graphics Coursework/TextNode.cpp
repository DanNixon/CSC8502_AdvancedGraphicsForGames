/** @file */

#include "TextNode.h"

#include "TextMesh.h"

namespace GraphicsCoursework
{
  TextNode::TextNode(const std::string & name, Font * font, size_t len)
    : MeshNode(name, new TextMesh(len, font))
  {
  }

  TextNode::~TextNode()
  {
  }

  void TextNode::SetText(const std::string & text)
  {
    ((TextMesh *)m_mesh)->SetText(text);
  }
}