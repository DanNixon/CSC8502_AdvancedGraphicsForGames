/** @file */

#pragma once

#include <../nclgl/GameTimer.h>
#include <../nclgl/Vector2.h>

#include "WindowsSystemMonitor.h"

namespace GraphicsCoursework
{
class Renderer;
class SceneNode;
class FramebufferNode;
class PointLight;
class SpotLight;
class MatrixNode;
class ParticleSystemNode;

/**
 * @brief Structure holding data describing world state and pointers to nodes required to update the world accordingly.
 */
struct WorldState
{
  WorldState()
    : screenBuffer(nullptr)
    , worldBounds(5000.0f)
    , worldClockSpeed((1.0f / 1000.0f) / 60.0f) // 1 day = 60 seconds
    , timeOfDay(0.25f)
    , colourTemp(3000.0f)
    , colourTempTarget(3000.0f)
    , colourTempSpeed(10.0f)
      , loadingNode(nullptr)
      , sun(nullptr)
      , moon(nullptr)
      , lantern(nullptr)
      , flashlight(nullptr)
      , waterTexMatrix(nullptr)
      , rain(nullptr)
      , snow(nullptr)
    , explosionStartTime(-1.0f)
    , explosion(nullptr)
    , cameraShakeTimeRemainingMs(0.0f)
    , cameraShakeIntensity(0.0f)
    , cameraShakeSpeed(40.0f)
    , filterAmount(0.0f)
  {
  }

  FramebufferNode *screenBuffer; //!< Framebuffer used to render scene to

  WindowsSystemMonitor sysMonitor; //!< Performance monitor
  GameTimer sysMonitorTimer;       //!< Timer used to update performance monitor

  Vector2 screenDims; //!< Dimensions of the renderer window

  const float worldBounds; //!< World size (used for defining size of certain components)

  float worldClockSpeed; //!< Speed of world clock tick relative to real clock tick
  float timeOfDay;       //!< Time of day (0.0 - 1.0)

  float colourTemp; //!< Colour temperature factor used in post processing  
  float colourTempTarget; //!< Target colour temperature to reach
  float colourTempSpeed; //!< Speed of colour temperature changes

  SceneNode *loadingNode;     //!< Loading text node
  PointLight *sun;            //!< Sun light
  PointLight *moon;           //!< Moon light
  PointLight *lantern;        //!< Player lantern
  SpotLight *flashlight;      //!< Player torch
  MatrixNode *waterTexMatrix; //!< Water surface texture matrix

  ParticleSystemNode *rain; //!< Rain particle system
  ParticleSystemNode *snow; //!< Snow particle system

  float explosionStartTime; //!< Time that the explosion was triggered
  ParticleSystemNode *explosion; //!< Explosion particle system
  float cameraShakeTimeRemainingMs; //!< TIme in milliseconds left for the camera shake effect
  float cameraShakeIntensity; //!< Intensity of camera shake effect
  float cameraShakeSpeed; //!< Speed of camera shake effect
  float filterAmount; //!< Amount of filter texture to blend into final frame
};

/**
 * @class World
 * @brief Used to manage setup of the world and hold its state.
 */
class World
{
public:
  World(Renderer &renderer);

  void BuildLoadingScreen(SceneNode *root);
  void Build(SceneNode *root);

  void Update(float msec);

  /**
   * @brief Gets the world state.
   * @return Reference to world state
   */
  inline WorldState &State()
  {
    return m_state;
  }

protected:
  Renderer &m_renderer; //!< Renderer being used
  WorldState m_state;   //!< World state
};
}
