/** @file */

#include "Light.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
Light::Light(const std::string &name)
    : ShaderDataNode(name)
    , m_radius(1.0f)
    , m_ambientIntensity(0.1f)
    , m_colour(1.0f, 1.0f, 1.0f, 1.0f)
{
  m_shaderUniformNames[UNIFORM_POSITION] = name + "_position";
  m_shaderUniformNames[UNIFORM_RADIUS] = name + "_radius";
  m_shaderUniformNames[UNIFORM_COLOUR] = name + "_colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = name + "_ambientIntensity";
}

Light::~Light()
{
}

void Light::PreRender(RenderState &state)
{
  ShaderDataNode::PreRender(state);
  state.numLights++;
}

void Light::PostRender(RenderState &state)
{
  ShaderDataNode::PostRender(state);
  state.numLights--;
}

void Light::ShaderBind(ShaderProgram *s)
{
  glUniform3fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_POSITION].c_str()),
               1, (float *)&m_worldTransform.GetPositionVector());

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_RADIUS].c_str()),
              m_radius);

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1,
               (float *)&m_colour);

  glUniform1f(
      glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY].c_str()),
      m_ambientIntensity);
}
}