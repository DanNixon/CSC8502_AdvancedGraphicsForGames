/** @file */

#pragma once

#include "ShaderDataNode.h"

namespace GraphicsCoursework
{
/**
 * @class ILight
 * @brief Base class containing common functionality for working with lights in a scene.
 */
class ILight : public ShaderDataNode
{
public:
  /**
   * @brief Enumeration of shader uniforms used for all types of light.
   */
  enum ShaderUniforms
  {
    UNIFORM_POSITION = 0,
    UNIFORM_COLOUR,
    UNIFORM_AMBIENT_INTENSITY,
    UNIFORM_REACH,

    UNIFORM_DIRECTION,
    UNIFORM_CUTOFF,

    UNIFORM_COUNT
  };

public:
  ILight(const std::string &name);
  virtual ~ILight();

  /**
   * @brief Gets the index of this light in the lights array.
   * @return Light index
   */
  inline size_t Index() const
  {
    return m_index;
  }

  virtual void SetIndex(size_t index);

  /**
   * @brief Gets or sets the colour of this light.
   * @return Light colour
   */
  inline Vector4 &Colour()
  {
    return m_colour;
  }

  /**
   * @brief Gets or sets the intensity of ambient light contribution from this light.
   * @return Ambient light intensity
   */
  inline float &AmbientIntensity()
  {
    return m_ambientIntensity;
  }

  /**
   * @brief Gets or sets the maximum distance this light can reach.
   * @return Reach
   */
  inline float &Reach()
  {
    return m_reach;
  }

  virtual size_t NumDirections() const = 0;
  virtual void CastDirections(std::vector<Vector3> &directions) const = 0;

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  virtual void SetUniformNames(const std::string &idx) = 0;

protected:
  size_t m_index;                                  //!< Index of this light in the shader light uniform array
  std::string m_shaderUniformNames[UNIFORM_COUNT]; //!< Cache of uniform names

  Vector4 m_colour;         //!< Light colour
  float m_ambientIntensity; //!< Contribution of this light to ambient light
  float m_reach;            //!< Distance this light can travel/reach
};
}
