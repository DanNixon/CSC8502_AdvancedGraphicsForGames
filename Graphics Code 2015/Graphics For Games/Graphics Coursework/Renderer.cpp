#include "Renderer.h"

#include "Camera.h"
#include "MeshNode.h"
#include "SceneNode.h"

namespace GraphicsCoursework
{
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
    , m_pvMatrix(new Matrix4())
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  m_pvMatrix->ToIdentity();

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;
  delete m_pvMatrix;
}

void Renderer::UpdateRenderList()
{
  m_sceneGraphRoot->UpdateTransformations();

  m_camera = nullptr;
  m_transparentNodes.clear();

  // TODO: find active camera
  // TODO: find and sort transparent
}

void Renderer::RenderScene()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera matrix
  *m_pvMatrix = m_camera->GetProjectionViewMatrix();

  // Render scene graph
  m_sceneGraphRoot->Render();

  // Render transparent nodes
  for (auto it = m_transparentNodes.begin(); it != m_transparentNodes.end(); ++it)
    (*it)->Draw();

  SwapBuffers();
}
}