#include "Renderer.h"

Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(nullptr)
{
  // TODO

  this->init = true;
}

Renderer::~Renderer()
{
  if (m_sceneGraphRoot != nullptr)
    delete m_sceneGraphRoot;
}

void Renderer::RenderScene()
{
  // TODO
}