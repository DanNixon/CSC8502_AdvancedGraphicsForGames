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

void ShaderNode::PreRender(RenderState & state)
{
    glGetIntegerv(GL_CURRENT_PROGRAM, &m_prevProgram);
    glUseProgram(m_program->Program());
    state.shader = m_program;

    glUniformMatrix4fv(glGetUniformLocation(m_program->Program(), "viewMatrix"), 1, false,
      (float *)&state.viewMatrix);
    glUniformMatrix4fv(glGetUniformLocation(m_program->Program(), "projMatrix"), 1, false,
      (float *)&state.projectionMatrix);
}

void ShaderNode::PostRender(RenderState & state)
{
    glUseProgram(m_prevProgram);
    state.shader = nullptr;
}
}
