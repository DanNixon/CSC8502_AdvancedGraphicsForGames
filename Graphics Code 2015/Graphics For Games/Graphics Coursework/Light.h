/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
class Light : public ShaderDataNode
{
public:
  Light(const std::string &name);
  virtual ~Light();

  inline float Radius() const
  {
    return m_radius;
  }

  virtual void SetRadius(float radius)
  {
    m_radius = radius;
  }

  inline Vector3 Colour() const
  {
    return m_colour;
  }

  virtual void SetColour(Vector3 colour)
  {
    m_colour = colour;
  }

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s);

protected:
  float m_radius;
  Vector3 m_colour;
};
}