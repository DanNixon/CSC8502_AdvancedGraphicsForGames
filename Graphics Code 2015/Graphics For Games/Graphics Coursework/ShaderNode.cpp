/** @file */

#include "ShaderNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
ShaderNode::ShaderNode(const std::string &name, ShaderProgram *program)
    : SceneNode(name)
    , m_program(program)
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

void ShaderNode::Render(RenderState &state)
{
  GLint prevProgram = 0;

  if (m_active)
  {
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
    glUseProgram(m_program->Program());
    state.shader = m_program;

    glUniformMatrix4fv(glGetUniformLocation(m_program->Program(), "viewMatrix"), 1, false,
                       (float *)&state.viewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(m_program->Program(), "projMatrix"), 1, false,
                       (float *)&state.projectionMatrix);
  }

  SceneNode::Render(state);

  if (m_active)
  {
    glUseProgram(prevProgram);
    state.shader = nullptr;
  }
}
}
