/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class ShaderDataNode
 * @brief Abstract class for nodes that hold data relevant to the shader that needs to be updated when the shader is
 *        selected or just before rendering.
 */
class ShaderDataNode : public SceneNode
{
public:
  ShaderDataNode(const std::string &name);
  virtual ~ShaderDataNode();

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  /**
   * @brief Called before rendering on entering a ShaderSyncNode.
   * @param s Pointer to the current shader program
   */
  virtual void ShaderBind(ShaderProgram *s)
  {
  }

  /**
   * @brief Called after rendering on exiting a ShaderSyncNode.
   * @param s Pointer to the current shader program
   */
  virtual void ShaderUnBind(ShaderProgram *s)
  {
  }
};
};
