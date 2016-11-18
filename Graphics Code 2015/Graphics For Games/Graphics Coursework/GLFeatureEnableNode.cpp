/** @file */

#include "GLFeatureEnableNode.h"

#include "GL/glew.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
GLFeatureEnableNode::GLFeatureEnableNode(const std::string &name, GLenum feature, GLboolean setState)
    : ShaderDataNode(name)
    , m_featureEnum(feature)
{
}

GLFeatureEnableNode::~GLFeatureEnableNode()
{
}

void GLFeatureEnableNode::ShaderBind(ShaderProgram *s)
{
  m_previousState = glIsEnabled(m_featureEnum);

  if (m_setState)
    glEnable(m_featureEnum);
  else
    glDisable(m_featureEnum);
}

void GLFeatureEnableNode::ShaderUnBind(ShaderProgram *s)
{
  if (m_previousState)
    glEnable(m_featureEnum);
  else
    glDisable(m_featureEnum);
}
}
