/** @file */

#pragma once

#include "RenderableNode.h"

#include "../nclgl/Mesh.h"

namespace GraphicsCoursework
{
class MeshNode : public RenderableNode
{
public:
  MeshNode(const std::string &name, Mesh *mesh, bool transparent = false);
  virtual ~MeshNode();

  inline Mesh *GetMesh()
  {
    return m_mesh;
  }

  virtual void Draw(RenderState &state);

protected:
  Mesh *m_mesh;
};
}
