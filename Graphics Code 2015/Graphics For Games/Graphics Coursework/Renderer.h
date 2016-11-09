#pragma once

#include "../../nclgl/OGLRenderer.h"

#include "SceneNode.h"
#include "Camera.h"

namespace GraphicsCoursework
{
struct RenderList
{
  Camera * camera;
  std::vector<SceneNode *> nodes;
};

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer();

  inline SceneNode *Root()
  {
    return m_sceneGraphRoot;
  }

  void UpdateRenderLists();

  virtual void RenderScene();

protected:
  SceneNode *m_sceneGraphRoot;
};
}