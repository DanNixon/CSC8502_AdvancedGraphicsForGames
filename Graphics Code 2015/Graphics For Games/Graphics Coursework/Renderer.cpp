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

  // POST PROCESSING
  {
    glGenTextures(1, &m_bufferDepthTex);
    glBindTexture(GL_TEXTURE_2D, m_bufferDepthTex);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    for (int i = 0; i < 2; ++i)
    {
      glGenTextures(1, &m_bufferColourTex[i]);
      glBindTexture(GL_TEXTURE_2D, m_bufferColourTex[i]);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }

    glGenFramebuffers(1, &m_bufferFBO);  // We ’ll render the scene into this
    glGenFramebuffers(1, &m_processFBO); // And do post processing in this

    glBindFramebuffer(GL_FRAMEBUFFER, m_bufferFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_bufferDepthTex, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferColourTex[0], 0);

    // We can check FBO attachment success using this command !
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !m_bufferDepthTex ||
        !m_bufferColourTex[0])
      return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  this->init = true;
}

Renderer::~Renderer()
{
  delete m_sceneGraphRoot;

  glDeleteTextures(2, m_bufferColourTex);
  glDeleteTextures(1, &m_bufferDepthTex);
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
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferColourTex[1], 0);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  SetCurrentShader(processShader);
  projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();

  glDisable(GL_DEPTH_TEST);

  glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

  for (int i = 0; i < POST_PASSES; ++i)
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferColourTex[1], 0);
    glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

    quad->SetTexture(m_bufferColourTex[0]);
    quad->Draw();

    glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_bufferColourTex[0], 0);

    quad->SetTexture(m_bufferColourTex[1]);
    quad->Draw();
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glUseProgram(0);

  glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  SetCurrentShader(sceneShader);
  projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
  viewMatrix.ToIdentity();
  UpdateShaderMatrices();
  quad->SetTexture(bufferColourTex[0]);
  quad->Draw();

  glUseProgram(0);
}

std::ostream &operator<<(std::ostream &s, const Renderer &r)
{
  s << "Renderer scene graph:\n";
  r.m_sceneGraphRoot->PrettyPrint(s);
  return s;
}
}
