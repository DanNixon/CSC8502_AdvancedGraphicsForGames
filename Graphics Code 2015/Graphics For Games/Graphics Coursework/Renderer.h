#pragma once

#include "../../nclgl/OGLRenderer.h"

#include "SceneNode.h"
#include "Camera.h"

namespace GraphicsCoursework
{
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