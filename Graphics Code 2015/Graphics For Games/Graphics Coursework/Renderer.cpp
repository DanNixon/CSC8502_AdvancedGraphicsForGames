/** @file */

#include "Renderer.h"

#include <algorithm>

#include "RenderableNode.h"
#include "RenderState.h"

namespace GraphicsCoursework
{
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
{
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
  std::sort(state.transparentNodes.begin(), state.transparentNodes.end(),
            [](RenderableNode * a, RenderableNode * b) { return a->CameraDistance() > b->CameraDistance(); });

  // Render transparent nodes
  for (auto it = state.transparentNodes.begin(); it != state.transparentNodes.end(); ++it)
    (*it)->RenderSingle();

  SwapBuffers();
}
}
