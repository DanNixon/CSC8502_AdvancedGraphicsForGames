/** @file */

#include "ProjectionNode.h"

namespace GraphicsCoursework
{
ProjectionNode::ProjectionNode(const std::string &name, const Matrix4 &proj)
    : SceneNode(name)
    , m_projectionMatrix(proj)
{
}

ProjectionNode::~ProjectionNode()
{
}

void ProjectionNode::PreRender(RenderState & state)
{
    state.projectionMatrix = m_projectionMatrix;
}

void ProjectionNode::PostRender(RenderState & state)
{
    state.projectionMatrix.ToIdentity();
}
}
