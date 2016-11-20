/** @file */

#include "ILight.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
ILight::ILight(const std::string &name)
    : ShaderDataNode(name)
    , m_colour(1.0f, 1.0f, 1.0f, 1.0f)
    , m_ambientIntensity(0.1f)
    , m_reach(10.0f)
{
}

ILight::~ILight()
{
}

void ILight::SetIndex(size_t index)
{
  m_index = index;
  SetUniformNames(std::to_string(m_index));
}

void ILight::ShaderBind(ShaderProgram *s)
{
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_POSITION].c_str()), 1,
               (float *)&m_worldTransform.GetPositionVector());

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1, (float *)&m_colour);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY].c_str()),
              m_ambientIntensity);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_REACH].c_str()), m_reach);
}

void ILight::ShaderUnBind(ShaderProgram *s)
{
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_POSITION].c_str()), 1,
               (float *)&Vector3());

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1,
               (float *)&Vector4(0.0f, 0.0f, 0.0f, 0.0f));

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY].c_str()), 0.0f);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_REACH].c_str()), 0.0f);
}
}