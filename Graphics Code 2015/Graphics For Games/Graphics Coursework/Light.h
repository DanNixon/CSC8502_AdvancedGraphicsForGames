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

  inline Vector3 Colour() const
  {
    return m_colour;
  }

  virtual void SetColour(Vector3 colour)
  {
    m_colour = colour;
  }

  // TODO

protected:
  Vector3 m_colour;
};
}