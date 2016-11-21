/** @file */

#include "SubTreeNode.h"

namespace GraphicsCoursework
{
  SubTreeNode::SubTreeNode(const std::string & name, SceneNode * subtree)
    : SceneNode(name)
    , m_oldParent(nullptr)
  {
    m_children.push_back(subtree);
  }

  SubTreeNode::~SubTreeNode()
  {
  }

  void SubTreeNode::PreRender(RenderState & state)
  {
    m_oldParent = m_children[0]->m_parent;
    m_children[0]->m_parent = this;
  }

  void SubTreeNode::PostRender(RenderState & state)
  {
    m_children[0]->m_parent = m_oldParent;
    m_oldParent = nullptr;
  }
}