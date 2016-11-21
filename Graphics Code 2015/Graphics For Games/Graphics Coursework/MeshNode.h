/** @file */

#pragma once

#include "RenderableNode.h"

#include "../nclgl/Mesh.h"

namespace GraphicsCoursework
{
/**
 * @class MeshNode
 * @brief Renderbale scene node that is bound to a Mesh.
 */
class MeshNode : public RenderableNode
{
public:
  MeshNode(const std::string &name, Mesh *mesh, bool transparent = false);
  virtual ~MeshNode();

  /**
   * @brief Returns a pointer to the mesh.
   * @return Mesh
   */
  inline Mesh *GetMesh()
  {
    return m_mesh;
  }

  virtual void Draw(RenderState &state);

protected:
  Mesh *m_mesh; //!< Mesh for this node
};
}
