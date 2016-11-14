#include "ISystemMonitor.h"

#include <iomanip>
#include <string>

namespace GraphicsCoursework
{
const std::vector<std::string> ISystemMonitor::METRIC_NAMES = {
    "Total Physical Memory (MB)",
    "Used Physical Memory (MB)",
    "Process Used Physical Memory (MB)",
    "Peak Process Used Physical Memory (MB)",
    "Total Virtual Memory (MB)",
    "Used Virtual Memory (MB)",
    "Process Used Virtual Memory (MB)",
    "Peak Process Used Virtual Memory (MB)",
    "Process CPU Usage",
    "Interval Averaged FPS",
    "Interval Averaged ms/Frame"};

ISystemMonitor::ISystemMonitor()
    : m_haveNewData(true)
    , m_frameCount(0)
{
  for (size_t i = 0; i < METRIC_COUNT; i++)
    m_metrics[i] = 0.0f;
}

ISystemMonitor::~ISystemMonitor()
{
}

bool ISystemMonitor::HaveNewData()
{
  bool retVal = m_haveNewData;
  m_haveNewData = false;
  return retVal;
}

void ISystemMonitor::Update(float dTimeMs)
{
  // Frame rate averaging
  m_metrics[ISystemMonitor::AVERAGE_MS_PER_FRAME] = dTimeMs / ((float)m_frameCount);
  m_metrics[ISystemMonitor::AVERAGE_FRAMES_PER_SECOND] = (m_frameCount * 1000.0f) / dTimeMs;
  m_frameCount = 0;

  // Mark data as new
  m_haveNewData = true;
}

std::ostream &operator<<(std::ostream &s, const ISystemMonitor &o)
{
  s << "Performance Metrics:\n";
  for (size_t i = 0; i < ISystemMonitor::METRIC_COUNT; i++)
    s << ' ' << std::left << std::setw(50) << ISystemMonitor::METRIC_NAMES[i] << " = "
      << o.m_metrics[i] << '\n';

  return s;
}
}