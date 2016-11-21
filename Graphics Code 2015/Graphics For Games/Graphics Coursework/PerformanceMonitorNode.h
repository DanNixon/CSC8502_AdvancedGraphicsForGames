/** @file */

#pragma once

#include "TextNode.h"

namespace GraphicsCoursework
{
class ISystemMonitor;

/**
 * @class PerformanceMonitorNode
 * @brief Renderable scene node that shows data from an ISystemMonitor.
 */
class PerformanceMonitorNode : public TextNode
{
public:
  PerformanceMonitorNode(const std::string &name, Font *font, ISystemMonitor *monitor);
  virtual ~PerformanceMonitorNode();

  virtual void Update(float msec);

protected:
  ISystemMonitor *m_monitor; //!< System monitor that provides data for this node
};
}
