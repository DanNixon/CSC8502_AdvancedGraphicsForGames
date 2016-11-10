#pragma once

#include <vector>

#include "../nclgl/Matrix4.h"

#include "ShaderProgram.h"

namespace GraphicsCoursework
{
struct SceneNodeRenderData
{
  SceneNodeRenderData()
    : program(nullptr)
  {
  }

  ShaderProgram *program;
};

class SceneNode
{
public:
  SceneNode(const std::string &name);
  virtual ~SceneNode();

  inline std::string Name() const
  {
    return m_name;
  }

  void AddChild(SceneNode *child);
  bool RemoveChild(SceneNode *child);
  bool RemoveChild(const std::string &name);

  SceneNode * Child(const std::string &name);

  inline void SetLocalTransformation(const Matrix4 &t)
  {
    m_localTransform = t;
  }

  inline Matrix4 GetLocalTransformation() const
  {
    return m_localTransform;
  }

  inline Matrix4 GetWorldTransformation() const
  {
    return m_worldTransform;
  }

  void UpdateTransformations();

  virtual void Render();

protected:
  const std::string m_name;
  SceneNode *m_parent;
  Matrix4 m_localTransform;
  Matrix4 m_worldTransform;
  std::vector<SceneNode *> m_children;

  SceneNodeRenderData m_renderData;
};
}