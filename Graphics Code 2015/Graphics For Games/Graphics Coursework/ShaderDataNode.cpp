/** @file */

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Create a new shader data node.
 * @param name Node name
 */
ShaderDataNode::ShaderDataNode(const std::string &name)
    : SceneNode(name)
{
}

ShaderDataNode::~ShaderDataNode()
{
}

/**
 * @copydoc SceneNode::PreRender
 */
void ShaderDataNode::PreRender(RenderState &state)
{
  state.shaderDataNodeStack.push_back(this);
}

/**
 * @copydoc SceneNode::PostRender
 */
void ShaderDataNode::PostRender(RenderState &state)
{
  state.shaderDataNodeStack.pop_back();
}
}
