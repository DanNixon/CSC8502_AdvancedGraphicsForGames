/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
  class SubTreeNode : public SceneNode
  {
  public:
    SubTreeNode(const std::string &name, SceneNode *subtree);
    virtual ~SubTreeNode();

    virtual void PreRender(RenderState &state);
    virtual void PostRender(RenderState &state);

  protected:
    SceneNode *m_oldParent;
  };
}