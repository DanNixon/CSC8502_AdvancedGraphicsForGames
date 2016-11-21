/** @file */

#pragma once

#include "ILight.h"

namespace GraphicsCoursework
{
/**
 * @class SpotLight
 * @brief Specialisation of ILight for shaped directional light sources.
 */
class SpotLight : public ILight
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

  /**
   * @copydoc ILight::NumDirections
   */
  virtual size_t NumDirections() const
  {
    return 1;
  }

  /**
   * @copydoc ILight::CastDirections
   */
  virtual void CastDirections(std::vector<Vector3> &directions) const
  {
    directions.push_back(Direction());
  }

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  virtual void SetUniformNames(const std::string &idx);

protected:
  float m_cutoff; //!< Spot cutoff value
};
}
