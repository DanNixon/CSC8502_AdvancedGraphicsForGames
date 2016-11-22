/** @file */

#include "ISystemMonitor.h"

#include <iomanip>
#include <string>

namespace GraphicsCoursework
{
/**
 * @brief List of names of all metrics (used for printing).
 */
const std::vector<std::string> ISystemMonitor::METRIC_NAMES = {"Total Physical Memory (MB)",
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

/**
 * @brief Creates a new system monitor.
 */
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

/**
 * @brief Checks if there is new data to read.
 * @return True if new data is available since last call to this function
 *
 * New data flag is reset every time this function is called.
 */
bool ISystemMonitor::HaveNewData()
{
  bool retVal = m_haveNewData;
  m_haveNewData = false;
  return retVal;
}

/**
 * @brief Performs measurements of monitored values.
 * @param dTimeMs Time in milliseconds that has passed since last call to this function
 */
void ISystemMonitor::Update(float dTimeMs)
{
  // Frame rate averaging
  m_metrics[ISystemMonitor::AVERAGE_MS_PER_FRAME] = dTimeMs / ((float)m_frameCount);
  m_metrics[ISystemMonitor::AVERAGE_FRAMES_PER_SECOND] = (m_frameCount * 1000.0f) / dTimeMs;
  m_frameCount = 0;

  // Mark data as new
  m_haveNewData = true;
}

/**
 * @brief Output data recorded by system monitor to a stream.
 * @param s Reference to output stream
 * @param o Reference to system monitor
 * @return Reference to output stream
 */
std::ostream &operator<<(std::ostream &s, const ISystemMonitor &o)
{
  s << "Performance Metrics:\n";
  for (size_t i = 0; i < ISystemMonitor::METRIC_COUNT; i++)
    s << ' ' << std::left << std::setw(50) << ISystemMonitor::METRIC_NAMES[i] << " = " << o.m_metrics[i] << '\n';

  return s;
}
}
