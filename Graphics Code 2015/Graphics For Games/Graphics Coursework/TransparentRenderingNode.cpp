/** @file */

#include "TransparentRenderingNode.h"

#include <algorithm>

#include "RenderableNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new transparent node render node.
 * @param name Node name
 */
TransparentRenderingNode::TransparentRenderingNode(const std::string &name)
    : SceneNode(name)
{
}

TransparentRenderingNode::~TransparentRenderingNode()
{
}

/**
 * @copydoc SceneNode::PreRender
 */
void TransparentRenderingNode::PreRender(RenderState &state)
{
  state.transparentNodes.clear();
}

/**
 * @copydoc SceneNode::PostRender
 */
void TransparentRenderingNode::PostRender(RenderState &state)
{
  state.transparentSync = this;

  /* Sort transparent nodes */
  std::sort(state.transparentNodes.begin(), state.transparentNodes.end(),
            [](RenderableNode *a, RenderableNode *b) { return a->CameraDistance() > b->CameraDistance(); });

  /* Render transparent nodes */
  for (auto it = state.transparentNodes.begin(); it != state.transparentNodes.end(); ++it)
    (*it)->RenderSingle(state);

  state.transparentNodes.clear();
  state.transparentSync = nullptr;
}
}
