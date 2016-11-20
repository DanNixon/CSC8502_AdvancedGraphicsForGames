/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class RenderableNode : public SceneNode
{
public:
  RenderableNode(const std::string &name, bool transparent = false);
  virtual ~RenderableNode();

  inline bool Transparent() const
  {
    return m_transparent;
  }

  inline size_t &RepeatedDraw()
  {
    return m_repeatedDraw;
  }

  inline float CameraDistance() const
  {
    return m_cameraDistance;
  }

  inline float BoundingSphereRadius() const
  {
    return m_boundingSphereRadius;
  }

  virtual void SetBoundingSphereRadius(float radius)
  {
    m_boundingSphereRadius = radius;
  }

  inline float &SpecularPower()
  {
    return m_specularPower;
  }

  inline float &SpecularIntensity()
  {
    return m_specularIntensity;
  }

  virtual void RenderSingle(RenderState &state);
  virtual void Draw(RenderState &state);

  virtual void PreRender(RenderState &state);

protected:
  bool m_transparent;
  size_t m_repeatedDraw;

  float m_cameraDistance;
  float m_boundingSphereRadius;

  float m_specularPower;
  float m_specularIntensity;
};
}
