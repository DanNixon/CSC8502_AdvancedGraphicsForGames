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

void PointLight::CastDirections(std::vector<Vector3> &directions) const
{
  directions.push_back(Vector3(1.0f, 0.0f, 0.0f));
  directions.push_back(Vector3(-1.0f, 0.0f, 0.0f));
  directions.push_back(Vector3(0.0f, 1.0f, 0.0f));
  directions.push_back(Vector3(0.0f, -1.0f, 0.0f));
  directions.push_back(Vector3(0.0f, 0.0f, 1.0f));
  directions.push_back(Vector3(0.0f, 0.0f, -1.0f));
}

void PointLight::SetUniformNames(const std::string &idx)
{
  m_shaderUniformNames[UNIFORM_POSITION] = "pointLights[" + idx + "].light.position";
  m_shaderUniformNames[UNIFORM_COLOUR] = "pointLights[" + idx + "].light.colour";
  m_shaderUniformNames[UNIFORM_AMBIENT_INTENSITY] = "pointLights[" + idx + "].light.ambientIntensity";
  m_shaderUniformNames[UNIFORM_REACH] = "pointLights[" + idx + "].light.reach";
}
}