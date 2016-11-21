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

struct WorldState
{
  WorldState()
      : screenBuffer(nullptr)
      , worldBounds(5000.0f)
      , worldClockSpeed((1.0f / 1000.0f) / 60.0f) // 1 day = 60 seconds
      , timeOfDay(0.0f)
      , loadingNode(nullptr)
      , sun(nullptr)
      , moon(nullptr)
      , lantern(nullptr)
      , flashlight(nullptr)
      , waterTexMatrix(nullptr)
  {
  }

  FramebufferNode *screenBuffer;

  WindowsSystemMonitor sysMonitor;
  GameTimer sysMonitorTimer;

  Vector2 screenDims;

  const float worldBounds;

  float worldClockSpeed;
  float timeOfDay;

  SceneNode *loadingNode;
  PointLight *sun;
  PointLight *moon;
  PointLight *lantern;
  SpotLight *flashlight;
  MatrixNode *waterTexMatrix;
};

class World
{
public:
  World(Renderer &renderer);

  void BuildLoadingScreen(SceneNode *root);
  void Build(SceneNode *root);

  void Update(float msec);

  inline WorldState &State()
  {
    return m_state;
  }

protected:
  Renderer &m_renderer;
  WorldState m_state;
};
}