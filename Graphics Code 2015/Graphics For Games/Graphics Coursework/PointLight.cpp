/** @file */

#include "PointLight.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
  PointLight::PointLight(const std::string &name)
    : ILight(name)
    , m_radius(1.0f)
{
}

  PointLight::~PointLight()
{
}

void PointLight::PreRender(RenderState &state)
{
  ShaderDataNode::PreRender(state);
  state.scenePointLights.push_back(this);
}

void PointLight::PostRender(RenderState &state)
{
  ShaderDataNode::PostRender(state);
  state.scenePointLights.pop_back();
}

void PointLight::ShaderBind(ShaderProgram *s)
{
  ILight::ShaderBind(s);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_RADIUS].c_str()), m_radius);
}

void PointLight::ShaderUnBind(ShaderProgram *s)
{
  ILight::ShaderBind(s);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_RADIUS].c_str()), 0.0f);
}

void PointLight::SetUniformNames(const std::string & idx)
{
  m_shaderUniformNames[UNIFORM_POSITION] = "pointLights[" + idx + "].light.position";
  m_shaderUniformNames[UNIFORM_COLOUR] = "pointLights[" + idx + "].light.colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = "pointLights[" + idx + "].light.ambientIntensity";

  m_shaderUniformNames[UNIFORM_RADIUS] = "pointLights[" + idx + "].radius";
}
}