/** @file */

#include "ShaderNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
ShaderNode::ShaderNode(const std::string &name, ShaderProgram *program)
    : SceneNode(name)
    , m_program(program)
    , m_previousProgram(nullptr)
{
  SetActive(true);
}

ShaderNode::~ShaderNode()
{
}

void ShaderNode::SetActive(bool active)
{
  m_active = active && m_program != nullptr && m_program->Valid();
}

void ShaderNode::PreRender(RenderState &state)
{
  m_previousProgram = state.shader;
  state.shader = m_program;
  glUseProgram(m_program->Program());

  glUniformMatrix4fv(glGetUniformLocation(m_program->Program(), "viewMatrix"), 1, false,
                     (float *)&state.viewMatrix);
}

void ShaderNode::PostRender(RenderState &state)
{
  if (m_previousProgram != nullptr)
    glUseProgram(m_previousProgram->Program());
  else
    glUseProgram(0);

  state.shader = m_previousProgram;
}
}
