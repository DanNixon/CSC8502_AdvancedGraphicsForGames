/** @file */

#include "PointLight.h"

#include "Renderer.h"
#include "ShaderProgram.h"

namespace GraphicsCoursework
{
  PointLight::PointLight(const std::string &name)
    : ILight(name)
{
}

  PointLight::~PointLight()
{
}

void PointLight::SetUniformNames(const std::string & idx)
{
  m_shaderUniformNames[UNIFORM_POSITION] = "pointLights[" + idx + "].light.position";
  m_shaderUniformNames[UNIFORM_COLOUR] = "pointLights[" + idx + "].light.colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = "pointLights[" + idx + "].light.ambientIntensity";
  m_shaderUniformNames[UNIFORM_REACH] = "pointLights[" + idx + "].light.reach";
}
}