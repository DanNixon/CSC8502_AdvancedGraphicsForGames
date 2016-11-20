/** @file */

#pragma once

#include "Light.h"

namespace GraphicsCoursework
{
/**
 * @class SpotLight
 * @brief Specialisation of Light for shaped directional light sources.
 */
class SpotLight : public Light
{
public:
  SpotLight(const std::string &name);
  virtual ~SpotLight();

  /**
   * @brief Gets the direction the light is facing.
   * @return Light direction.
   */
  inline Vector3 Direction() const
  {
    return -m_worldTransform.GetBackVector();
  }
};
}
