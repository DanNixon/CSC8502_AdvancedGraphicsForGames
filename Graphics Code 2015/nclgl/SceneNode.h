#pragma once

#include <vector>

#include "Matrix4.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Vector4.h"

class SceneNode
{
public:
  static bool CompareByCameraDistance(SceneNode *a, SceneNode *b)
  {
    return (a->m_distanceFromCamera < b->m_distanceFromCamera) ? true : false;
  }

public:
  SceneNode(Mesh *m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
  ~SceneNode(void);

  void SetTransform(const Matrix4 &matrix)
  {
    m_transform = matrix;
  }

  inline const Matrix4 &GetTransform() const
  {
    return m_transform;
  }

  inline Matrix4 GetWorldTransform() const
  {
    return m_worldTransform;
  }

  inline Vector4 GetColour() const
  {
    return m_colour;
  }

  inline void SetColour(Vector4 c)
  {
    m_colour = c;
  }

  inline Vector3 GetModelScale() const
  {
    return m_modelScale;
  }

  inline void SetModelScale(Vector3 s)
  {
    m_modelScale = s;
  }

  inline Mesh *GetMesh() const
  {
    return m_mesh;
  }

  inline void SetMesh(Mesh *m)
  {
    m_mesh = m;
  }

  void AddChild(SceneNode *s);
  void RemoveChild(SceneNode *s);

  virtual void Update(float msec);
  virtual void Draw(const OGLRenderer &r);

  inline std::vector<SceneNode *>::const_iterator GetChildIteratorStart()
  {
    return m_children.begin();
  }

  inline std::vector<SceneNode *>::const_iterator GetChildIteratorEnd()
  {
    return m_children.end();
  }

  inline float GetBoundingRadius() const
  {
    return m_boundingRadius;
  }

  inline void SetBoundingRadius(float f)
  {
    m_boundingRadius = f;
  }

  inline float GetCameraDistance() const
  {
    return m_distanceFromCamera;
  }

  inline void SetCameraDistance(float f)
  {
    m_distanceFromCamera = f;
  }

protected:
  SceneNode *m_parent;
  Mesh *m_mesh;
  Matrix4 m_worldTransform;
  Matrix4 m_transform;
  Vector3 m_modelScale;
  Vector4 m_colour;
  std::vector<SceneNode *> m_children;
  float m_distanceFromCamera;
  float m_boundingRadius;
};
