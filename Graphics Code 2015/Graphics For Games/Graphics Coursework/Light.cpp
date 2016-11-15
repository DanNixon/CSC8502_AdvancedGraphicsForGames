/** @file */

#include "Light.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
Light::Light(const std::string &name)
    : ShaderDataNode(name)
    , m_radius(1.0f)
{
  m_shaderUniformNames[UNIFORM_POSITION] = name + "_position";
  m_shaderUniformNames[UNIFORM_RADIUS] = name + "_radius";
  m_shaderUniformNames[UNIFORM_COLOUR] = name + "_colour";
}

Light::~Light()
{
}

void Light::AddChild(SceneNode *child)
{
  ShaderDataNode::AddChild(child);
  m_renderer->m_state.shaderDataNodeStack.push_back(this);
}

bool Light::RemoveChild(SceneNode *child)
{
  auto it = std::find(m_renderer->m_state.shaderDataNodeStack.begin(),
                      m_renderer->m_state.shaderDataNodeStack.end(), this);

  if (it != m_renderer->m_state.shaderDataNodeStack.end())
    m_renderer->m_state.shaderDataNodeStack.erase(it);

  return ShaderDataNode::RemoveChild(child);
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
               1, (float *)&m_localTransform);

  glUniform1f(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_RADIUS].c_str()),
              m_radius);

  glUniform4fv(glGetUniformLocation(s->Program(), m_shaderUniformNames[UNIFORM_COLOUR].c_str()), 1,
               (float *)&m_colour);
}
}