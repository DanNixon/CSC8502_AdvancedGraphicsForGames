#pragma once

#include "SceneNode.h"

#include "../nclgl/Mesh.h"

namespace GraphicsCoursework
{
  //TODO: testing only
  class Texture;

class MeshNode : public SceneNode
{
public:
  MeshNode(const std::string &name, Mesh *mesh, bool transparent = false);
  virtual ~MeshNode();

  virtual void Render(RenderState & state);
  virtual void RenderSingle(RenderState & state);

private:
  bool m_transparent;
  Mesh *m_mesh;

  //TODO: testing only
public:
  Texture *tex;
};
}