#include "ShaderNode.h"

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
  bool canUse = m_program != nullptr && m_program->valid();
  m_use = use ? canUse : false;
}

void ShaderNode::Render()
{
  if (m_use)
  {
    // TODO
  }

  SceneNode::Render();

  if (m_use)
  {
    // TODO
  }
}