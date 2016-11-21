/** @file */

#include "TextureNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new texture mapping node.
 * @param name Node name
 * @param mappings List of texture mappings
 */
TextureNode::TextureNode(const std::string &name, const TextureMappingList &mappings)
    : ShaderDataNode(name)
    , m_mappings(mappings)
{
}

TextureNode::~TextureNode()
{
}

/**
 * @copydoc ShaderDataNode::ShaderBind
 */
void TextureNode::ShaderBind(ShaderProgram *s)
{
  for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
    it->Map(s->Program());
}

/**
 * @copydoc ShaderDataNode::ShaderUnBind
 */
void TextureNode::ShaderUnBind(ShaderProgram *s)
{
  for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
    it->UnMap(s->Program());
}
}
