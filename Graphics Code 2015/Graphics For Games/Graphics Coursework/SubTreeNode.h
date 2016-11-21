/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class SubTreeNode
 * @brief Used to process a subtree of the scene graph under this node, essentially allowing a subtree to appear in
 *        multiple positions in the scene graph.
 *
 * Note that only one child should be added to this node.
 */
class SubTreeNode : public SceneNode
{
public:
  SubTreeNode(const std::string &name, SceneNode *subtree);
  virtual ~SubTreeNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

protected:
  SceneNode *m_oldParent; //!< Old parent of the subtree root node
};
}
