#pragma once

#include "../../nclgl/OGLRenderer.h"

namespace GraphicsCoursework
{
class SceneNode;
class Camera;
class MeshNode;

class Renderer : public OGLRenderer
{
public:
  Renderer(Window &parent);
  virtual ~Renderer();

  inline SceneNode *Root()
  {
    return m_sceneGraphRoot;
  }

  inline Matrix4 GetProjectionViewMatrix() const
  {
    return *m_pvMatrix;
  }

  void UpdateRenderList();

  virtual void RenderScene();

protected:
  SceneNode *m_sceneGraphRoot;
  Matrix4 *m_pvMatrix;

  Camera *m_camera;
  std::vector<MeshNode *> m_transparentNodes;
};
}