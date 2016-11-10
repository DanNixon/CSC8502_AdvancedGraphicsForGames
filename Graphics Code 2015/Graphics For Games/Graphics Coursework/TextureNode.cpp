/** @file */

#include "TextureNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
  TextureNode::TextureNode(const std::string & name, const TextureMappingList & mappings)
    : SceneNode(name)
    , m_mappings(mappings)
  {
  }

  TextureNode::~TextureNode()
  {
  }

  void TextureNode::Render(RenderState & state)
  {
    if (m_active)
    {
      for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
        it->Map(state.shader->Program());
    }

    SceneNode::Render(state);

    if (m_active)
    {
      for (auto it = m_mappings.begin(); it != m_mappings.end(); ++it)
        it->UnMap(state.shader->Program());
    }
  }
}