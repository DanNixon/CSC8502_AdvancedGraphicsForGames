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

  virtual void AddPersistentDataNode(ShaderDataNode *node);
  virtual bool RemovePersistentDataNode(ShaderDataNode *node);

  virtual void RenderScene();

  friend std::ostream &operator<<(std::ostream &s, const Renderer &r);

protected:
  SceneNode *m_sceneGraphRoot;
  RenderState m_state;
};
}
