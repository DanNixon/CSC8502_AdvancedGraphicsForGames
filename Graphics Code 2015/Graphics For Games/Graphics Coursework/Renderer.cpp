/** @file */

#include "Renderer.h"

#include <algorithm>

#include "RenderableNode.h"

namespace GraphicsCoursework
{
Renderer::Renderer(Window &parent)
    : OGLRenderer(parent)
    , m_sceneGraphRoot(new SceneNode("root", this))
    , m_postProcessingGraphRoot(new SceneNode("postProcessRoot", this))
    , m_postProcessingPresentationGraphRoot(new SceneNode("postProcessPresentRoot", this))
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenFramebuffers(1, &m_bufferFBO);
  glGenFramebuffers(1, &m_processFBO);

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;

  glDeleteFramebuffers(1, &m_bufferFBO);
  glDeleteFramebuffers(1, &m_processFBO);
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
  DrawScene();
  DrawPostProcess();
  PresentScene();
  SwapBuffers();
}

void Renderer::DrawScene()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_bufferFBO);

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render graph
  m_sceneGraphRoot->Render(m_state);

  // Sort transparent nodes
  std::sort(m_state.transparentNodes.begin(), m_state.transparentNodes.end(),
            [](RenderableNode *a, RenderableNode *b) { return a->CameraDistance() > b->CameraDistance(); });

  // Render transparent nodes
  for (auto it = m_state.transparentNodes.begin(); it != m_state.transparentNodes.end(); ++it)
    (*it)->RenderSingle(m_state);

  m_state.transparentNodes.clear();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawPostProcess()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_processFBO);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);

  RenderState state;
  m_postProcessingGraphRoot->Render(state);

  glEnable(GL_DEPTH_TEST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::PresentScene()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  RenderState state;
  m_postProcessingPresentationGraphRoot->Render(state);
  glUseProgram(0);
}

std::ostream &operator<<(std::ostream &s, const Renderer &r)
{
  s << "Renderer scene graph:\n";
  r.m_sceneGraphRoot->PrettyPrint(s);
  return s;
}
}
