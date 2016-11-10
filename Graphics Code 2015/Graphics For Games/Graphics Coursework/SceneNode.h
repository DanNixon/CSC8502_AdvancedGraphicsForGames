#pragma once

#include <vector>

#include "../nclgl/Matrix4.h"

#include "RenderState.h"

namespace GraphicsCoursework
{
class ShaderProgram;
class Renderer;

class SceneNode
{
public:
  SceneNode(const std::string &name);
  virtual ~SceneNode();

  inline std::string Name() const
  {
    return m_name;
  }

  inline bool IsActive() const
  {
    return m_active;
  }

  virtual void SetActive(bool active)
  {
    m_active = active;
  }

  void AddChild(SceneNode *child);
  bool RemoveChild(SceneNode *child);
  bool RemoveChild(const std::string &name);

  SceneNode * Child(const std::string &name);

  virtual void SetLocalTransformation(const Matrix4 &t)
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

  virtual void Render(RenderState & state);

protected:
  const std::string m_name;

  Renderer *m_renderer;
  SceneNode *m_parent;

  bool m_active;

  Matrix4 m_localTransform;
  Matrix4 m_worldTransform;

  std::vector<SceneNode *> m_children;
};
}