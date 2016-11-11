/** @file */

#include "ProjectionNode.h"

#include "GL/glew.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
ProjectionNode::ProjectionNode(const std::string &name, const Matrix4 &proj)
    : ShaderDataNode(name)
    , m_projectionMatrix(proj)
{
}

ProjectionNode::~ProjectionNode()
{
}

void ProjectionNode::ShaderBind(ShaderProgram *s)
{
  glUniformMatrix4fv(glGetUniformLocation(s->Program(), "projMatrix"), 1, false,
                     (float *)&m_projectionMatrix);
}
}
