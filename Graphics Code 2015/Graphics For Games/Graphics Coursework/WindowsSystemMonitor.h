/** @file */

#pragma once

#include "ISystemMonitor.h"

#include <windows.h>

namespace GraphicsCoursework
{
/**
 * @class WindowsSystemMonitor
 * @brief System monitor for the Windows OS.
 *
 * Some code taken from:
 * http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 */
class WindowsSystemMonitor : public ISystemMonitor
{
public:
  WindowsSystemMonitor();
  virtual ~WindowsSystemMonitor();

  virtual void Update(float dTimeMs);

protected:
  size_t m_numProcessors; //!< Number of processor cores

  HANDLE m_currentProcessHandle; //!< Handle to the current process

  ULARGE_INTEGER m_lastCPU;       //!< Last measured CPU time for this process
  ULARGE_INTEGER m_lastSystemCPU; //!< Last measured system CPU time for this process
  ULARGE_INTEGER m_lastUserCPU;   //!< Last measured user CPU time for this process
};
}
