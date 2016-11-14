/** @file */

#pragma once

#include "TextNode.h"

namespace GraphicsCoursework
{
class PerformanceMonitorNode : public TextNode
{
public:
  PerformanceMonitorNode(const std::string &name, Font *font);
  virtual ~PerformanceMonitorNode();

  virtual void Update(float msec);
};
}
