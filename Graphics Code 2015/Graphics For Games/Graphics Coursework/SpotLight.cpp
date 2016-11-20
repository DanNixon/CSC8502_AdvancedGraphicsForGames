/** @file */

#include "SpotLight.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
SpotLight::SpotLight(const std::string &name)
    : ILight(name)
    , m_cutoff(0.0f)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::PreRender(RenderState &state)
{
  ILight::PreRender(state);
  state.sceneSpotLights.push_back(this);
}

void SpotLight::PostRender(RenderState &state)
{
  ILight::PostRender(state);
  state.sceneSpotLights.pop_back();
}

void SpotLight::ShaderBind(ShaderProgram *s)
{
  ILight::ShaderBind(s);

  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_DIRECTION].c_str()), 1,
    (float *)&Direction());

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_CUTOFF].c_str()), m_cutoff);
}

void SpotLight::ShaderUnBind(ShaderProgram *s)
{
  ILight::ShaderBind(s);

  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_DIRECTION].c_str()), 1,
    (float *)&Matrix4());

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_CUTOFF].c_str()), 0.0f);
}

void SpotLight::SetUniformNames(const std::string & idx)
{
  m_shaderUniformNames[UNIFORM_POSITION] = "spotLights[" + idx + "].light.position";
  m_shaderUniformNames[UNIFORM_COLOUR] = "spotLights[" + idx + "].light.colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = "spotLights[" + idx + "].light.ambientIntensity";

  m_shaderUniformNames[UNIFORM_DIRECTION] = "spotLights[" + idx + "].direction";
  m_shaderUniformNames[UNIFORM_CUTOFF] = "spotLights[" + idx + "].cutoff";
}
}
