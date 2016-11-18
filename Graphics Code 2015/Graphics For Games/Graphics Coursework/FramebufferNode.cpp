/** @file */

#include "FramebufferNode.h"

#include "ITexture.h"

namespace GraphicsCoursework
{
FramebufferNode::FramebufferNode(const std::string &name, bool generate)
    : SceneNode(name)
    , m_buffer(0)
{
  if (generate)
    glGenFramebuffers(1, &m_buffer);
}

FramebufferNode::~FramebufferNode()
{
  glDeleteFramebuffers(1, &m_buffer);
}

bool FramebufferNode::Valid() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
  bool retVal = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return retVal;
}

void FramebufferNode::BindTexture(GLuint target, ITexture *texture)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, target, texture->Type(), texture->GetTextureID(), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FramebufferNode::PreRender(RenderState &state)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void FramebufferNode::PostRender(RenderState &state)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}