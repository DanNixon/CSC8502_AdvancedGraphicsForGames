/** @file */

#include "GenericControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new generic control node.
 * @param name Node name
 * @param onBind Function called when entering this node
 * @param onUnBind Function called when exiting this node
 */
GenericControlNode::GenericControlNode(const std::string &name, ControlCallbackFunc onBind,
                                       ControlCallbackFunc onUnBind)
    : ShaderDataNode(name)
    , m_onBind(onBind)
    , m_onUnBind(onUnBind)
{
}

GenericControlNode::~GenericControlNode()
{
}

void GenericControlNode::ShaderBind(ShaderProgram *s)
{
  m_onBind(s);
}

void GenericControlNode::ShaderUnBind(ShaderProgram *s)
{
  m_onUnBind(s);
}
}
