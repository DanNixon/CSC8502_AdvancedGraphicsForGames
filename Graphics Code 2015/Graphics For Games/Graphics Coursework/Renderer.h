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

  inline SceneNode *PostProcessingRoot()
  {
    return m_postProcessingGraphRoot;
  }

  virtual void AddPersistentDataNode(ShaderDataNode *node);
  virtual bool RemovePersistentDataNode(ShaderDataNode *node);

  virtual void RenderScene();

  friend std::ostream &operator<<(std::ostream &s, const Renderer &r);

protected:
  virtual void DrawScene();
  virtual void DrawPostProcess();
  virtual void PresentScene();

protected:
  RenderState m_state;

  SceneNode *m_sceneGraphRoot;
  SceneNode *m_postProcessingGraphRoot;

  GLuint m_bufferFBO;
  GLuint m_processFBO;
  GLuint m_bufferColourTex[2];
  GLuint m_bufferDepthTex;
};
}
