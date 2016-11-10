#include "Renderer.h"

#include "RenderState.h"
#include "MeshNode.h"

namespace GraphicsCoursework
{
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
{
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;
}

void Renderer::RenderScene()
{
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Update transformations
  m_sceneGraphRoot->UpdateTransformations();

  RenderState state;

  // Render graph
  m_sceneGraphRoot->Render(state);

  // Sort transparent nodes
  // TODO

  // Render transparent nodes
  for (auto it = state.transparentNodes.begin(); it != state.transparentNodes.end(); ++it)
  {
    RenderState s;
    (*it)->RenderSingle(s);
  }

  SwapBuffers();
}
}