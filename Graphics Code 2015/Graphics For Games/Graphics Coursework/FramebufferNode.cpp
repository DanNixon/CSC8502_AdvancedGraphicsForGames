/** @file */

#include "FramebufferNode.h"

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