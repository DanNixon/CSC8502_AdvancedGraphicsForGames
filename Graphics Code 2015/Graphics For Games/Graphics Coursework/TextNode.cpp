/** @file */

#include "TextNode.h"

#include "TextMesh.h"

namespace GraphicsCoursework
{
TextNode::TextNode(const std::string &name, Font *font, size_t len)
    : MeshNode(name, new TextMesh(len, font))
{
}

TextNode::TextNode(const std::string & name, Font * font, const std::string & text)
  : MeshNode(name, new TextMesh(text.size(), font))
{
  SetText(text);
}

TextNode::~TextNode()
{
}

void TextNode::SetText(const std::string &text)
{
  ((TextMesh *)m_mesh)->SetText(text);
}
}