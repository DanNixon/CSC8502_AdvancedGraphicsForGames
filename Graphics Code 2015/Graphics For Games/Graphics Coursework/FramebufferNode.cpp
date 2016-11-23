/** @file */

#include "FramebufferNode.h"

#include "ITexture.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new framebuffer node.
 * @param name Node name
 * @param generate True to generate a new framebuffer
 *
 * Setting generate to false will set this node to be the default (screen/back) buffer.
 */
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

/**
 * @brief Tests the validity of the underlying framebuffer.
 * @return True if framebuffer is complete
 */
bool FramebufferNode::Valid() const
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
  bool retVal = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return retVal;
}

/**
 * @brief Binds a texture to the framebuffer.
 * @param target Texture type
 * @param texture Texture
 */
void FramebufferNode::BindTexture(GLuint target, ITexture *texture)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, target, texture->Type(), texture->GetTextureID(), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @copydoc SceneNode::PreRender
 */
void FramebufferNode::PreRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  }
}

/**
 * @copydoc SceneNode::PostRender
 */
void FramebufferNode::PostRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
}
