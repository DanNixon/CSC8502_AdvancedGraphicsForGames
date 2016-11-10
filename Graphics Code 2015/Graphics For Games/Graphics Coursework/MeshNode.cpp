#include "MeshNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
MeshNode::MeshNode(const std::string &name, Mesh *mesh, ShaderProgram *shader, bool transparent)
    : SceneNode(name)
    , m_transparent(transparent)
    , m_mesh(mesh)
    , m_shader(shader)
{
}

MeshNode::~MeshNode()
{
  delete m_mesh;
}

void MeshNode::Render()
{
  if (m_active && !m_transparent)
    Draw();

  SceneNode::Render();
}

void MeshNode::Draw()
{
  m_shader->Use(m_renderer);
  m_mesh->Draw();
}
}