/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class ITexture;

class FramebufferNode : public SceneNode
{
public:
  FramebufferNode(const std::string &name, bool generate = true);
  virtual ~FramebufferNode();

  inline GLuint Buffer()
  {
    return m_buffer;
  }

  bool Valid() const;

  void BindTexture(GLuint target, ITexture *texture);

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

protected:
  GLuint m_buffer;
};
}