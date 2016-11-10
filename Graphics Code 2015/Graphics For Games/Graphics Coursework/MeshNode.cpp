#include "MeshNode.h"

namespace GraphicsCoursework
{
MeshNode::MeshNode(const std::string &name, Mesh *mesh)
    : SceneNode(name)
    , m_mesh(mesh)
{
}

MeshNode::~MeshNode()
{
  delete m_mesh;
}

void MeshNode::Render()
{
  m_mesh->Draw();
}
}