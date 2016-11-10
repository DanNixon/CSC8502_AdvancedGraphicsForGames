/** @file */

#include "MeshNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
MeshNode::MeshNode(const std::string &name, Mesh *mesh, bool transparent)
    : SceneNode(name)
    , m_transparent(transparent)
    , m_mesh(mesh)
{
}

MeshNode::~MeshNode()
{
  delete m_mesh;
}

void MeshNode::Render(RenderState & state)
{
  if (m_active)
  {
    if (!m_transparent)
    {
      // Set model matrix
      if (state.shader != nullptr)
        glUniformMatrix4fv(glGetUniformLocation(state.shader->Program(), "modelMatrix"), 1, false, (float*)&m_worldTransform);

      // Draw mesh
      m_mesh->Draw();
    }
    else
    {
      state.transparentNodes.push_back(this);
    }
  }

  SceneNode::Render(state);
}

void MeshNode::RenderSingle(RenderState & state)
{
  // TODO
}
}