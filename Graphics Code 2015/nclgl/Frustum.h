#pragma once

#include "Matrix4.h"
#include "Plane.h"
#include "SceneNode.h"

class Matrix4;

class Frustum
{
public:
  Frustum(){};
  ~Frustum(){};

  void FromMatrix(const Matrix4 &mvp);
  bool InsideFrustum(SceneNode &n);

protected:
  Plane m_planes[6];
};
