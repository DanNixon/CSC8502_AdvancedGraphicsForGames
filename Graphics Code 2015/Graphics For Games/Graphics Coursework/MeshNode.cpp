/** @file */

#include "MeshNode.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
/**
 * @brief Creates a new renderable mesh node.
 * @param name Node name
 * @param mesh Pointer to mesh data
 * @param transparent Flag indicating if the mesh and texture combination will result in transparent fragments
 */
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

/**
 * @copydoc RenderableNode::Draw
 */
void MeshNode::Draw(RenderState &state)
{
  RenderableNode::Draw(state);
  m_mesh->Draw();
}
}
