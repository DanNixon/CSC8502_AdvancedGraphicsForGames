#include "ProjectionNode.h"

namespace GraphicsCoursework
{
  ProjectionNode::ProjectionNode(const std::string & name, const Matrix4 &proj)
    : SceneNode(name), m_projectionMatrix(proj)
  {
  }

  ProjectionNode::~ProjectionNode()
  {
  }

  void ProjectionNode::Render(RenderState & state)
  {
    if (m_active)
      state.projectionMatrix = m_projectionMatrix;

    SceneNode::Render(state);

    if (m_active)
      state.projectionMatrix.ToIdentity();
  }
}