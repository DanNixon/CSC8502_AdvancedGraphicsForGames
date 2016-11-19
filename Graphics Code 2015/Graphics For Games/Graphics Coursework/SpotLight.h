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

  inline Vector3 Direction() const
  {
    return -m_worldPosition.GetBackVector();
  }
};
}
