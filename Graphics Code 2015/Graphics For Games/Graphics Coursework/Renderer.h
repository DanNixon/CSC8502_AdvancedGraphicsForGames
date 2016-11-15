/** @file */

#pragma once

#include "../../nclgl/OGLRenderer.h"

#include "RenderState.h"

namespace GraphicsCoursework
{
class SceneNode;

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer();

  inline SceneNode *Root()
  {
    return m_sceneGraphRoot;
  }

  virtual void RenderScene();

protected:
  friend class Light;

  SceneNode *m_sceneGraphRoot;
  RenderState m_state;
};
}
