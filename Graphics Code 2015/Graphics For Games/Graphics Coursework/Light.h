/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
class Light : public ShaderDataNode
{
public:
  enum ShaderUniforms
  {
    UNIFORM_POSITION,
    UNIFORM_RADIUS,
    UNIFORM_COLOUR,

    UNIFORM_COUNT
  };

public:
  Light(const std::string &name);
  virtual ~Light();

  inline std::string ShaderUniformName(ShaderUniforms uniform) const
  {
    return m_shaderUniformNames[uniform];
  }

  inline float &Radius()
  {
    return m_radius;
  }

  inline Vector3 &Colour()
  {
    return m_colour;
  }

  virtual void AddChild(SceneNode *child);
  virtual bool RemoveChild(SceneNode *child);

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s);

protected:
  std::string m_shaderUniformNames[UNIFORM_COUNT];

  float m_radius;
  Vector3 m_colour;
};
}