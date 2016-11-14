/** @file */

#pragma once

#include <vector>

namespace GraphicsCoursework
{
class ISystemMonitor
{
public:
  enum Metrics
  {
    PHYSICAL_MEMORY_TOTAL = 0,
    PHYSICAL_MEMORY_USED,
    PHYSICAL_MEMORY_SELF_USED,
    PHYSICAL_MEMORY_SELF_USED_PEAK,

    VIRTUAL_MEMORY_TOTAL,
    VIRTUAL_MEMORY_USED,
    VIRTUAL_MEMORY_SELF_USED,
    VIRTUAL_MEMORY_SELF_USED_PEAK,

    CPU_USAGE,
    CPU_SELF_USAGE,

    AVERAGE_FRAMES_PER_SECOND,
    AVERAGE_MS_PER_FRAME,

    METRIC_COUNT
  };

public:
  static const std::vector<std::string> METRIC_NAMES;

public:
  ISystemMonitor();
  virtual ~ISystemMonitor();

  virtual void Update();

  inline float Metric(Metrics m) const
  {
    return m_metrics[m];
  }

  friend std::ostream &operator<<(std::ostream &stream, const ISystemMonitor &o);

protected:
  float m_metrics[METRIC_COUNT];
};
}