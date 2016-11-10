#pragma once

#include "SceneNode.h"

#include "../nclgl/Mesh.h"

namespace GraphicsCoursework
{
class ShaderProgram;

class MeshNode : public SceneNode
{
public:
  MeshNode(const std::string &name, Mesh *mesh, ShaderProgram *shader, bool transparent = false);
  virtual ~MeshNode();

  inline bool IsTransparent() const
  {
    return m_transparent;
  }

  virtual void Render();
  void Draw();

private:
  bool m_transparent;
  ShaderProgram *m_shader;
  Mesh *m_mesh;
};
}