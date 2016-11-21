/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
class ILight : public ShaderDataNode
{
public:
  enum ShaderUniforms
  {
    UNIFORM_POSITION = 0,
    UNIFORM_COLOUR,
    UNIFORM_AMBIENT_INTENSITY,
    UNIFORM_REACH,

    UNIFORM_DIRECTION,
    UNIFORM_CUTOFF,

    UNIFORM_COUNT
  };

public:
  ILight(const std::string &name);
  virtual ~ILight();

  inline size_t Index() const
  {
    return m_index;
  }

  virtual void SetIndex(size_t index);

  inline Vector4 &Colour()
  {
    return m_colour;
  }

  inline float &AmbientIntensity()
  {
    return m_ambientIntensity;
  }

  inline float &Reach()
  {
    return m_reach;
  }

  virtual size_t NumDirections() const = 0;
  virtual void CastDirections(std::vector<Vector3> &directions) const = 0;

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  virtual void SetUniformNames(const std::string &idx) = 0;

protected:
  size_t m_index;
  std::string m_shaderUniformNames[UNIFORM_COUNT];

  Vector4 m_colour;
  float m_ambientIntensity;
  float m_reach;
};
}