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

  inline SceneNode *PostProcessingPresentationRoot()
  {
    return m_postProcessingPresentationGraphRoot;
  }

  inline GLuint BufferFBO()
  {
    return m_bufferFBO;
  }

  inline GLuint ProcessingFBO()
  {
    return m_processFBO;
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
  SceneNode *m_postProcessingPresentationGraphRoot;

  GLuint m_bufferFBO;
  GLuint m_processFBO;
};
}
