/** @file */

#pragma once

#include "ILight.h"

namespace GraphicsCoursework
{
class PointLight : public ILight
{
public:
  PointLight(const std::string &name);
  virtual ~PointLight();

  inline float &Radius()
  {
    return m_radius;
  }

  virtual void PreRender(RenderState &state);
  virtual void PostRender(RenderState &state);

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  virtual void SetUniformNames(const std::string &idx);

protected:
  float m_radius;
};
}