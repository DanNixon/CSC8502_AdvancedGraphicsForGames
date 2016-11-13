/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class RenderableNode : public SceneNode
{
public:
  RenderableNode(const std::string &name, bool transparent = false);
  virtual ~RenderableNode();

  inline bool Transparent() const
  {
    return m_transparent;
  }

  inline float CameraDistance() const
  {
    return m_cameraDistance;
  }

  virtual void RenderSingle();
  virtual void Draw(RenderState &state);

  virtual void PreRender(RenderState &state);

protected:
  bool m_transparent;
  float m_cameraDistance;
};
}
