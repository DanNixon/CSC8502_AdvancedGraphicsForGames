/** @file */

#include "SpotLight.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new spot light.
 * @param name Node name
 */
SpotLight::SpotLight(const std::string &name)
    : ILight(name)
    , m_cutoff(0.8f)
{
  m_ambientIntensity = 0.0f;
}

SpotLight::~SpotLight()
{
}

/**
 * @copydoc ILight::ShaderBind
 */
void SpotLight::ShaderBind(ShaderProgram *s)
{
  ILight::ShaderBind(s);

  auto d = Direction();
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_DIRECTION].c_str()), 1, (float *)&d);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_CUTOFF].c_str()), m_cutoff);
}

/**
 * @copydoc ILight::ShaderUnBind
 */
void SpotLight::ShaderUnBind(ShaderProgram *s)
{
  ILight::ShaderUnBind(s);

  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_DIRECTION].c_str()), 1,
               (float *)&Matrix4());

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_CUTOFF].c_str()), 0.0f);
}

/**
 * @copydoc ILight::SetUniformNames
 */
void SpotLight::SetUniformNames(const std::string &idx)
{
  m_shaderUniformNames[UNIFORM_POSITION] = "spotLights[" + idx + "].light.position";
  m_shaderUniformNames[UNIFORM_COLOUR] = "spotLights[" + idx + "].light.colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = "spotLights[" + idx + "].light.ambientIntensity";
  m_shaderUniformNames[UNIFORM_REACH] = "spotLights[" + idx + "].light.reach";

  m_shaderUniformNames[UNIFORM_DIRECTION] = "spotLights[" + idx + "].direction";
  m_shaderUniformNames[UNIFORM_CUTOFF] = "spotLights[" + idx + "].cutoff";
}
}
