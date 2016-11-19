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
  if (m_owner)
    delete[] m_program;
}

void ShaderNode::SetActive(bool active)
{
  m_active = active && m_program != nullptr && m_program->Valid();
}

void ShaderNode::PreRender(RenderState &state)
{
  m_previousProgram = state.shader;
  state.shader = m_program;
}

void ShaderNode::PostRender(RenderState &state)
{
  state.shader = m_previousProgram;
}
}
