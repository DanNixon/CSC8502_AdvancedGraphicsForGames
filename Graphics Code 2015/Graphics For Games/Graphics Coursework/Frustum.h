#pragma once

#include <../nclgl/Plane.h>
#include <../nclgl/Matrix4.h>

namespace GraphicsCoursework
{
  class SceneNode;

  class Frustum
  {
  public:
    Frustum();
    virtual ~Frustum();

    void FromMatrix(const Matrix4 &m);
    bool ContainsSceneNode(SceneNode &n);

  protected:
    Plane m_planes[6];
  };
}