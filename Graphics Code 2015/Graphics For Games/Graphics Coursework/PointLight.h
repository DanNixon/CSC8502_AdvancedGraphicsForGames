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

protected:
  virtual void SetUniformNames(const std::string &idx);
};
}