/** @file */

#include "SubTreeNode.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new subtree node.
 * @param name Node name
 * @param subtree Node to use as the root of the subtree
 */
SubTreeNode::SubTreeNode(const std::string &name, SceneNode *subtree)
    : SceneNode(name)
    , m_oldParent(nullptr)
{
  SetProcessMode(PM_DONT_CARE);
  m_children.push_back(subtree);
}

SubTreeNode::~SubTreeNode()
{
  m_children.clear();
}

/**
 * @copydoc SceneNode::PreRender
 */
void SubTreeNode::PreRender(RenderState &state)
{
  m_oldParent = m_children[0]->m_parent;
  m_children[0]->m_parent = this;
}

/**
 * @copydoc SceneNode::PostRender
 */
void SubTreeNode::PostRender(RenderState &state)
{
  m_children[0]->m_parent = m_oldParent;
  m_oldParent = nullptr;
}

/**
 * @copydoc SceneNode::PrettyPrint
 */
void SubTreeNode::PrettyPrint(std::ostream &s, size_t level) const
{
  SceneNode *subtreeRoot = m_children[0];

  s << " " << std::string(level, ' ') << " - " << (*this)
    << " (subtree root = " << (subtreeRoot == nullptr ? "null" : subtreeRoot->Name()) << ")\n";
}
}
