/** @file */

#include "PerformanceMonitorNode.h"

#include <sstream>
#include <iomanip>

#include "ISystemMonitor.h"

namespace GraphicsCoursework
{
PerformanceMonitorNode::PerformanceMonitorNode(const std::string &name, Font *font, ISystemMonitor *monitor)
    : TextNode(name, font, 64)
    , m_monitor(monitor)
{
}

PerformanceMonitorNode::~PerformanceMonitorNode()
{
}

void PerformanceMonitorNode::Update(float msec)
{
  TextNode::Update(msec);

  if (m_active)
  {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1)
      << "FPS: " << m_monitor->Metric(ISystemMonitor::AVERAGE_FRAMES_PER_SECOND) << ", "
      << "CPU: " << m_monitor->Metric(ISystemMonitor::CPU_SELF_USAGE) << ", "
      << std::setprecision(2)
      << "pMem: " << m_monitor->Metric(ISystemMonitor::PHYSICAL_MEMORY_SELF_USED) << "MB, "
      << "vMem: " << m_monitor->Metric(ISystemMonitor::VIRTUAL_MEMORY_SELF_USED) << "MB";

    SetText(ss.str());
  }
}
}
