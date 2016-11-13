#include "ISystemMonitor.h"

#include <string>
#include <iomanip>

namespace GraphicsCoursework
{
  const std::vector<std::string> ISystemMonitor::METRIC_NAMES =
  {
    "Total Physical Memory (MB)",
    "Used Physical Memory (MB)",
    "Process Used Physical Memory (MB)",
    "Peak Process Used Physical Memory (MB)",
    "Total Virtual Memory (MB)",
    "Used Virtual Memory (MB)",
    "Process Used Virtual Memory (MB)",
    "Peak Process Used Virtual Memory (MB)"
  };

  ISystemMonitor::ISystemMonitor()
  {
  }

  ISystemMonitor::~ISystemMonitor()
  {
  }

  std::ostream &operator<<(std::ostream &s, const ISystemMonitor &o)
  {
    s << "Performance Metrics:\n";
    for (size_t i = 0; i < ISystemMonitor::METRIC_COUNT; i++)
      s << ' ' << std::left << std::setw(50) << ISystemMonitor::METRIC_NAMES[i] << " = " << o.m_metrics[i] << '\n';

    return s;
  }
}