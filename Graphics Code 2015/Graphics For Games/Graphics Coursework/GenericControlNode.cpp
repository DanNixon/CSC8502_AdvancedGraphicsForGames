/** @file */

#include "GenericControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
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
