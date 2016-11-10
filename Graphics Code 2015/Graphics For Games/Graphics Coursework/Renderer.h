#pragma once

#include "../../nclgl/OGLRenderer.h"

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
  SceneNode *m_sceneGraphRoot;
};
}