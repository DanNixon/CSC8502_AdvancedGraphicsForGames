/** @file */

#include "TextureNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
TextureNode::TextureNode(const std::string &name, const TextureMappingList &mappings)
    : ShaderDataNode(name)
    , m_mappings(mappings)
{
}

TextureNode::~TextureNode()
{
}

void TextureNode::ShaderBind(ShaderProgram *s)
{
  for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
    it->Map(s->Program());
}

void TextureNode::ShaderUnBind(ShaderProgram *s)
{
  for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
    it->UnMap(s->Program());
}
}