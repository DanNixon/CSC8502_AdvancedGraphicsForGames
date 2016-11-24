/** @file */

#include "TreeControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
TreeControlNode::TreeControlNode(const std::string &name, ControlCallbackFunc onBind, ControlCallbackFunc onUnBind)
    : SceneNode(name)
    , m_onEntry(onBind)
    , m_onExit(onUnBind)
{
}

TreeControlNode::~TreeControlNode()
{
}

/**
 * @copydoc SceneNode::PreRender
 */
void TreeControlNode::PreRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    m_onEntry(state);
}

/**
 * @copydoc SceneNode::PostRender
 */
void TreeControlNode::PostRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    m_onExit(state);
}
}
