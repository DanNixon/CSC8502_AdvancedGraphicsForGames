#include "ShaderNode.h"

namespace GraphicsCoursework
{
ShaderNode::ShaderNode(const std::string &name, ShaderProgram *program)
    : SceneNode(name)
    , m_program(program)
{
  SetUse(true);
}

ShaderNode::~ShaderNode()
{
  if (m_program != nullptr)
    delete m_program;
}

void ShaderNode::SetUse(bool use)
{
  bool canUse = m_program != nullptr && m_program->Valid();
  m_use = use ? canUse : false;
}

void ShaderNode::Render()
{
  GLint prevProgram = 0;

  if (m_use)
  {
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);
    glUseProgram(m_program->Program());
  }

  SceneNode::Render();

  if (m_use)
    glUseProgram(prevProgram);
}
}