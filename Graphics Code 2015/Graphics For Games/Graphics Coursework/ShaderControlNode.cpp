/** @file */

#include "ShaderControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new generic control node.
 * @param name Node name
 * @param onBind Function called when entering this node
 * @param onUnBind Function called when exiting this node
 */
ShaderControlNode::ShaderControlNode(const std::string &name, ControlCallbackFunc onBind, ControlCallbackFunc onUnBind)
    : ShaderDataNode(name)
    , m_onBind(onBind)
    , m_onUnBind(onUnBind)
{
}

ShaderControlNode::~ShaderControlNode()
{
}

/**
 * @copydoc ShaderDataNode::ShaderBind
 */
void ShaderControlNode::ShaderBind(ShaderProgram *s)
{
  m_onBind(s);
}

/**
 * @copydoc ShaderDataNode::ShaderUnBind
 */
void ShaderControlNode::ShaderUnBind(ShaderProgram *s)
{
  m_onUnBind(s);
}
}
