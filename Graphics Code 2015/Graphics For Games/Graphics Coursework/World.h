/** @file */

#pragma once

#include <../nclgl/Vector2.h>
#include <../nclgl/GameTimer.h>

#include "WindowsSystemMonitor.h"

namespace GraphicsCoursework
{
  class Renderer;
  class SceneNode;
  class FramebufferNode;

  struct WorldState
  {
    WorldState()
      : screenBuffer(nullptr)
      , loadingNode(nullptr)
      , sun(nullptr)
      , moon(nullptr)
    {
    }

    FramebufferNode *screenBuffer;

    WindowsSystemMonitor sysMonitor;
    GameTimer sysMonitorTimer;

    Vector2 screenDims;

    float timeOfDay;

    SceneNode * loadingNode;
    SceneNode * sun;
    SceneNode * moon;
  };

  class World
  {
  public:
    World(Renderer &renderer);

    void BuildLoadingScreen(SceneNode * root);
    void Build(SceneNode * root);

    void Update(float msec);

    inline WorldState& State()
    {
      return m_state;
    }

  protected:
    Renderer &m_renderer;
    WorldState m_state;
  };
}