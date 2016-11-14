/** @file */

#pragma once

#include "ISystemMonitor.h"

#include <windows.h>

namespace GraphicsCoursework
{
class WindowsSystemMonitor : public ISystemMonitor
{
public:
  WindowsSystemMonitor();
  virtual ~WindowsSystemMonitor();

  virtual void Update(float dTimeMs);

protected:
  size_t m_numProcessors;

  HANDLE m_self;

  ULARGE_INTEGER m_lastCPU;
  ULARGE_INTEGER m_lastSystemCPU;
  ULARGE_INTEGER m_lastUserCPU;
};
}