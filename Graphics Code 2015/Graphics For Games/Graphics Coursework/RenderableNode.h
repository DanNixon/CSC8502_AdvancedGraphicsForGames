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

  /**
   * @brief Gets the value of the transparent flag for this node.
   * @return True if node is (partially) transparent
   */
  inline bool Transparent() const
  {
    return m_transparent;
  }

  /**
   * @brief Gets or sets the number of times this renderable is drawn in one render pass.
   * @return NUmber of repeated draws
   *
   * Useful for certain post processing effects.
   */
  inline size_t &RepeatedDraw()
  {
    return m_repeatedDraw;
  }

  /**
   * @brief Gets the cached camera distance for this node.
   * @return Distance to active camera
   */
  inline float CameraDistance() const
  {
    return m_cameraDistance;
  }

  /**
   * @brief Gets the radius of the bounding sphere around this renderable.
   * @return Bounding sphere radius
   */
  inline float BoundingSphereRadius() const
  {
    return m_boundingSphereRadius;
  }

  /**
   * @brief Sets the bounding sphere radius for this renderable.
   * @param radius New bounding sphere radius
   */
  virtual void SetBoundingSphereRadius(float radius)
  {
    m_boundingSphereRadius = radius;
  }

  /**
   * @brief Gets or sets the specular power for lighting this renderable.
   * @return Specular power
   */
  inline float &SpecularPower()
  {
    return m_specularPower;
  }

  /**
   * @brief Gets or sets the specular intensity for lighting this renderable.
   * @return Specular intensity
   */
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
