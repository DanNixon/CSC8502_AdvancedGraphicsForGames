/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
/**
 * @class RenderableNode
 * @brief Scene node that can be rendered in a scene.
 */
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
  bool m_transparent;    //!< Flag indicating if this node will be (partially) transparent
  size_t m_repeatedDraw; //!< Number of times this node will be drawn

  float m_cameraDistance;       //!< Cached distance from the active camera
  float m_boundingSphereRadius; //!< Radius of the bounding sphere used in view frustum culling (negative values disable
                                //   check)

  float m_specularPower;     //!< Specular power used in lighting calculations
  float m_specularIntensity; //!< Specular intensity used in lighting calculations
};
}
