/** @file */

#include "TreeControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new tree control node.
 * @param name Node name
 * @param onEntry Function called when entering this node
 * @param onExit Function called when exiting this node
 */
TreeControlNode::TreeControlNode(const std::string &name, ControlCallbackFunc onEntry, ControlCallbackFunc onExit)
    : SceneNode(name)
    , m_onEntry(onEntry)
    , m_onExit(onExit)
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
