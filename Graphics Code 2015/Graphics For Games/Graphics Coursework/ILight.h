/** @file */

#pragma once

#include "ShaderDataNode.h"

#include <../nclgl/Vector2.h>

#include "RGBATexture.h"

namespace GraphicsCoursework
{
class ShadowTexture;
class FramebufferNode;

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

  void InitShadows(GLuint shadowTexDim, SceneNode *shadowSceneRoot, const Vector2 &screenDims);

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

  /**
   * @brief Gets the number of directions this light is cast in.
   * @return Number of directions
   *
   * Should be equal to the number of render passes it would take to correctly shadow this light.
   */
  virtual size_t NumDirections() const = 0;

  /**
   * @brief Gets all directions this light casts light in.
   * @param directions Reference to output container
   */
  virtual void CastDirections(std::vector<Vector3> &directions) const = 0;

  void DoShadowRender(RenderState &mainState);

  virtual void ShaderBind(ShaderProgram *s);
  virtual void ShaderUnBind(ShaderProgram *s);

protected:
  /**
   * @brief Builds the uniform name cache for this light.
   * @param idx Index of this light in shader uniform array
   */
  virtual void SetUniformNames(const std::string &idx) = 0;

protected:
  size_t m_index;                                  //!< Index of this light in the shader light uniform array
  std::string m_shaderUniformNames[UNIFORM_COUNT]; //!< Cache of uniform names

  Vector4 m_colour;         //!< Light colour
  float m_ambientIntensity; //!< Contribution of this light to ambient light
  float m_reach;            //!< Distance this light can travel/reach

  std::vector<ShadowTexture *> m_shadowTextures; //!< Textures used in shadow mapping render passes
  FramebufferNode *m_shadowSceneRoot;            //!< Root node of the shadow rendering scene
  CameraNode *m_shadowCamera;                    //!< Camera used to render the shadow depth map
  Matrix4 m_shadowProjection;                    //!< Projection matrix used when rendering shadows
};
}
