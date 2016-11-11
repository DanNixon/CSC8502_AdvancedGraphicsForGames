/** @file */

#pragma once

#include "SceneNode.h"

namespace GraphicsCoursework
{
class PerformanceMonitorNode : public SceneNode
{
public:
  PerformanceMonitorNode(const std::string &name);
  virtual ~PerformanceMonitorNode();

  inline float FramesPerSecond() const
  {
  }

  inline float MillisecondsPerFrame() const
  {
    return m_lastMsPerFrame;
  }

  virtual void Update(float msec);

protected:
  void Average();

protected:
  float m_lastAverageMs;
  size_t m_numFramesInSample;

  float m_lastMsPerFrame;
};
}
