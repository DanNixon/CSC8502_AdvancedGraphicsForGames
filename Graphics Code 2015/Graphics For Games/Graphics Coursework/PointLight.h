/** @file */

#pragma once

#include "ILight.h"

namespace GraphicsCoursework
{
class PointLight : public ILight
{
public:
  PointLight(const std::string &name);
  virtual ~PointLight();

  virtual size_t NumDirections() const
  {
    return 6;
  }

  virtual void CastDirections(std::vector<Vector3> &directions) const;

protected:
  virtual void SetUniformNames(const std::string &idx);
};
}