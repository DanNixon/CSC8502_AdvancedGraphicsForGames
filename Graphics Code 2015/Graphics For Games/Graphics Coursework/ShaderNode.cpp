/** @file */

#include "ShaderNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new shader node.
 * @param name Node name
 * @param program Shader program to be activated by this node
 */
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
    delete m_program;
}

/**
 * @copydoc SceneNode::SetActive
 */
void ShaderNode::SetActive(bool active, bool recursive)
{
  m_active = active && m_program != nullptr && m_program->Valid();

  if (recursive)
  {
    for (auto it = m_children.begin(); it != m_children.end(); ++it)
      (*it)->SetActive(active, true);
  }
}

/**
 * @copydoc SceneNode::PreRender
 */
void ShaderNode::PreRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
  {
    m_previousProgram = state.shader;
    state.shader = m_program;
  }
}

/**
 * @copydoc SceneNode::PostRender
 */
void ShaderNode::PostRender(RenderState &state)
{
  if (ProcessingPassCheck(state))
    state.shader = m_previousProgram;
}
}
