/** @file */

#include "ShaderSyncNode.h"

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
  ShaderSyncNode::ShaderSyncNode(const std::string & name)
    : SceneNode(name)
  {
  }

  ShaderSyncNode::~ShaderSyncNode()
  {
  }

  void ShaderSyncNode::PreRender(RenderState & state)
  {
    for (auto it = state.shaderDataNodeStack.begin(); it != state.shaderDataNodeStack.end(); ++it)
      (*it)->ShaderBind(state.shader);
  }

  void ShaderSyncNode::PostRender(RenderState & state)
  {
    for (auto it = state.shaderDataNodeStack.rbegin(); it != state.shaderDataNodeStack.rend(); ++it)
      (*it)->ShaderUnBind(state.shader);
  }
}