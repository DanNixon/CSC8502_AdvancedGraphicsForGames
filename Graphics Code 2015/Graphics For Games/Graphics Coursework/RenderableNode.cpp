/** @file */

#include "RenderableNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
RenderableNode::RenderableNode(const std::string &name, bool transparent)
    : SceneNode(name)
    , m_transparent(transparent)
    , m_cameraDistance(0.0f)
{
}

RenderableNode::~RenderableNode()
{
}

void RenderableNode::RenderSingle(RenderState &state)
{
  std::vector<SceneNode *> stack;
  GetStack(stack);

  for (auto it = stack.rbegin(); it != stack.rend(); ++it)
    (*it)->PreRender(state);

  Draw(state);

  for (auto it = stack.begin(); it != stack.end(); ++it)
    (*it)->PostRender(state);
}

void RenderableNode::Draw(RenderState &state)
{
  // Process shader
  if (state.shader != nullptr)
  {
    // Set model matrix
    glUniformMatrix4fv(glGetUniformLocation(state.shader->Program(), "modelMatrix"), 1, false,
                       (float *)&m_worldTransform);
  }
}

void RenderableNode::PreRender(RenderState &state)
{
  if (!m_transparent)
  {
    Draw(state);
  }
  else
  {
    m_cameraDistance = DistanceFrom((SceneNode *)state.camera);
    state.transparentNodes.push_back(this);
  }
}
}