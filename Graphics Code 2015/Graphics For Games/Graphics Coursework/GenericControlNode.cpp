/** @file */

#include "GenericControlNode.h"

#include "GL/glew.h"

namespace GraphicsCoursework
{
GenericControlNode::GenericControlNode(const std::string &name)
    : ShaderDataNode(name)
    , m_onBind([](ShaderProgram *s) {})
    , m_onUnBind([](ShaderProgram *s) {})
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
