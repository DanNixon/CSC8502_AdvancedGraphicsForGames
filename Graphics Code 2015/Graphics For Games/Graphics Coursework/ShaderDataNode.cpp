/** @file */

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
ShaderDataNode::ShaderDataNode(const std::string & name)
  : SceneNode(name)
{
}

ShaderDataNode::~ShaderDataNode()
{
}

void ShaderDataNode::PreRender(RenderState & state)
{
  state.shaderDataNodeStack.push_back(this);
}

void ShaderDataNode::PostRender(RenderState & state)
{
  state.shaderDataNodeStack.pop_back();
}
}