/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class ITexture;

/**
 * @class FramebufferNode
 * @brief Scene node defining and binding a GL frame buffer.
 *
 * All child nodes will be rendered into this frame buffer.
 */
class FramebufferNode : public SceneNode
{
public:
  FramebufferNode(const std::string &name, bool generate = true);
  virtual ~FramebufferNode();

  /**
   * @brief Gets the GL framebufer handle.
   * @return GL buffer
   */
  inline GLuint Buffer()
  {
    return m_buffer;
  }

  bool Valid() const;

  void BindTexture(GLuint target, ITexture *texture);

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

protected:
  GLuint m_buffer; //!< GL buffer handle
};
}
