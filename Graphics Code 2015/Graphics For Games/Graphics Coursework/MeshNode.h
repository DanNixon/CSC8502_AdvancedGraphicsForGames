#pragma once

#include "SceneNode.h"

#include "../nclgl/Mesh.h"

namespace GraphicsCoursework
{
class MeshNode : public SceneNode
{
public:
  MeshNode(const std::string &name, Mesh *mesh);
  virtual ~MeshNode();

  virtual void Render();

private:
  Mesh *m_mesh;
};
}