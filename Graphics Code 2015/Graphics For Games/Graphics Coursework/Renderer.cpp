/** @file */

#include "Renderer.h"

#include <algorithm>

#include "RenderableNode.h"

namespace GraphicsCoursework
{
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;
}

void Renderer::AddPersistentDataNode(ShaderDataNode *node)
{
  m_state.shaderDataNodeStack.push_back(node);
}

bool Renderer::RemovePersistentDataNode(ShaderDataNode *node)
{
  auto it = std::find(m_state.shaderDataNodeStack.begin(), m_state.shaderDataNodeStack.end(), node);
  bool found = (it != m_state.shaderDataNodeStack.end());
  if (found)
    m_state.shaderDataNodeStack.erase(it);
  return found;
}

void Renderer::RenderScene()
{
  // Render graph
  m_sceneGraphRoot->Render(m_state);

  // Sort transparent nodes
  std::sort(m_state.transparentNodes.begin(), m_state.transparentNodes.end(),
            [](RenderableNode *a, RenderableNode *b) { return a->CameraDistance() > b->CameraDistance(); });

  // Render transparent nodes
  for (auto it = m_state.transparentNodes.begin(); it != m_state.transparentNodes.end(); ++it)
    (*it)->RenderSingle(m_state);

  m_state.transparentNodes.clear();

  SwapBuffers();
}

std::ostream &operator<<(std::ostream &s, const Renderer &r)
{
  s << "Renderer scene graph:\n";
  r.m_sceneGraphRoot->PrettyPrint(s);
  return s;
}
}
