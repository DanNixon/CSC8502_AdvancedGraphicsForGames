/** @file */

#include "MeshNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
MeshNode::MeshNode(const std::string &name, Mesh *mesh, bool transparent)
    : RenderableNode(name, transparent)
    , m_mesh(mesh)
{
}

MeshNode::~MeshNode()
{
  if (m_owner)
    delete m_mesh;
}

void MeshNode::Draw(RenderState &state)
{
  RenderableNode::Draw(state);
  m_mesh->Draw();
}
}