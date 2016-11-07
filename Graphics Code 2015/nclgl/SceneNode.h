#pragma once

#include <vector>

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"

class SceneNode
{
public:
  static bool CompareByCameraDistance(SceneNode *a, SceneNode *b)
  {
    return (a - > distanceFromCamera < b->distanceFromCamera) ? true : false;
  }

public:
  SceneNode(Mesh *m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
  ~SceneNode(void);

  void SetTransform(const Matrix4 &matrix)
  {
    transform = matrix;
  }

  inline const Matrix4 &GetTransform() const
  {
    return transform;
  }

  inline Matrix4 GetWorldTransform() const
  {
    return worldTransform;
  }

  inline Vector4 GetColour() const
  {
    return colour;
  }

  inline void SetColour(Vector4 c)
  {
    colour = c;
  }

  inline Vector3 GetModelScale() const
  {
    return modelScale;
  }

  inline void SetModelScale(Vector3 s)
  {
    modelScale = s;
  }

  inline Mesh *GetMesh() const
  {
    return mesh;
  }

  inline void SetMesh(Mesh *m)
  {
    mesh = m;
  }

  void AddChild(SceneNode *s);

  virtual void Update(float msec);
  virtual void Draw(const OGLRenderer &r);

  inline std::vector<SceneNode *>::const_iterator GetChildIteratorStart()
  {
    return children.begin();
  }

  inline std::vector<SceneNode *>::const_iterator GetChildIteratorEnd()
  {
    return children.end();
  }

  inline float GetBoundingRadius() const
  {
    return boundingRadius;
  }

  inline void SetBoundingRadius(float f)
  {
    boundingRadius = f;
  }

  inline float GetCameraDistance() const
  {
    return distanceFromCamera;
  }

  inline void SetCameraDistance(float f)
  {
    distanceFromCamera = f;
  }

protected:
  SceneNode *parent;
  Mesh *mesh;
  Matrix4 worldTransform;
  Matrix4 transform;
  Vector3 modelScale;
  Vector4 colour;
  std::vector<SceneNode *> children;
  float distanceFromCamera;
  float boundingRadius;
};
