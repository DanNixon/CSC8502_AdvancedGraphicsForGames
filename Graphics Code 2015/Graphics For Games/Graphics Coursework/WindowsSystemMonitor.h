#pragma once

#include "ISystemMonitor.h"

namespace GraphicsCoursework
{
class WindowsSystemMonitor : public ISystemMonitor
{
public:
  WindowsSystemMonitor();
  virtual ~WindowsSystemMonitor();

  virtual void Update();
};
}