/** @file */

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
  SceneNode(const std::string &name, Renderer *renderer = nullptr);
  virtual ~SceneNode();

  inline std::string Name() const
  {
    return m_name;
  }

  inline bool IsActive() const
  {
    return m_active;
  }

  virtual void SetActive(bool active, bool recursive = false);
  bool ToggleActive();

  inline bool IsOwner() const
  {
    return m_owner;
  }

  virtual void SetIsOwner(bool owner)
  {
    m_owner = owner;
  }

  void GetStack(std::vector<SceneNode *> &stack, SceneNode *until = nullptr);

  virtual SceneNode *AddChild(SceneNode *child);
  virtual bool RemoveChild(SceneNode *child);
  bool RemoveChild(const std::string &name);

  SceneNode *Child(const std::string &name);
  SceneNode *FindFirstChildByName(const std::string &name);

  virtual void SetLocalRotation(const Matrix4 &r)
  {
    m_localRotation = r;
  }

  inline Matrix4 GetLocalRotation() const
  {
    return m_localRotation;
  }

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

  /**
   * @brief Gets the distence between this node and another.
   * @param other Other node
   * @return Distance between nodes
   */
  inline float DistanceFrom(SceneNode *other) const
  {
    Vector3 dir = m_worldTransform.GetPositionVector() - other->m_worldTransform.GetPositionVector();
    return Vector3::Dot(dir, dir);
  }

  virtual void Update(float msec);
  virtual void Render(RenderState &state);

  /**
   * @brief Called when node is entered and before children are rendered.
   * @param state Reference to current render state
   */
  virtual void PreRender(RenderState &state)
  {
  }

  /**
   * @brief Called when children are rendered and before node is exited.
   * @param state Reference to current render state
   */
  virtual void PostRender(RenderState &state)
  {
  }

  void PrettyPrint(std::ostream &s, size_t level = 0) const;
  friend std::ostream &operator<<(std::ostream &s, const SceneNode &n);

protected:
  friend class SubTreeNode;

  const std::string m_name; //!< Node name

  Renderer *m_renderer; //!< Parent renderer
  SceneNode *m_parent;  //!< Parent node

  bool m_active; //!< Flag indicating if this node is active
  bool m_owner;  //!< Flag indicating that this node owns any held resources

  Matrix4 m_localRotation;  //!< Local rotation matrix
  Matrix4 m_localTransform; //!< Local transformation matrix

  Matrix4 m_worldTransform; //!< Cached world transformation matrix

  std::vector<SceneNode *> m_children; //!< Children of this node
};
}
