#pragma once

#include "../../nclgl/OGLRenderer.h"

class SceneNode;

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer();

  virtual void RenderScene();

protected:
  SceneNode *m_sceneGraphRoot;
};