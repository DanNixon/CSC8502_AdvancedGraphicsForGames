/** @file */

#include "WindowsSystemMonitor.h"

// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on

// http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

namespace GraphicsCoursework
{
const float BYTES_TO_MB = 1.0f / 1e6f;

WindowsSystemMonitor::WindowsSystemMonitor()
{
  SYSTEM_INFO sysInfo;
  FILETIME ftime, fsys, fuser;

  GetSystemInfo(&sysInfo);
  m_numProcessors = sysInfo.dwNumberOfProcessors;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&m_lastCPU, &ftime, sizeof(FILETIME));

  m_self = GetCurrentProcess();
  GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
  memcpy(&m_lastSystemCPU, &fsys, sizeof(FILETIME));
  memcpy(&m_lastUserCPU, &fuser, sizeof(FILETIME));
}

WindowsSystemMonitor::~WindowsSystemMonitor()
{
}

void WindowsSystemMonitor::Update(float dTimeMs)
{
  ISystemMonitor::Update(dTimeMs);

  // System wide memory
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);

  m_metrics[PHYSICAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPhys) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPageFile) * BYTES_TO_MB;

  m_metrics[PHYSICAL_MEMORY_USED] = ((float)(memInfo.ullTotalPhys - memInfo.ullAvailPhys)) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_USED] =
      ((float)(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile)) * BYTES_TO_MB;

  // Process memory
  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

  m_metrics[PHYSICAL_MEMORY_SELF_USED] = ((float)pmc.WorkingSetSize) * BYTES_TO_MB;
  m_metrics[PHYSICAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakWorkingSetSize) * BYTES_TO_MB;

  m_metrics[VIRTUAL_MEMORY_SELF_USED] = ((float)pmc.PagefileUsage) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakPagefileUsage) * BYTES_TO_MB;

  // CPU
  FILETIME ftime, fsys, fuser;
  ULARGE_INTEGER now, sys, user;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&now, &ftime, sizeof(FILETIME));

  GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser);
  memcpy(&sys, &fsys, sizeof(FILETIME));
  memcpy(&user, &fuser, sizeof(FILETIME));

  m_metrics[ISystemMonitor::CPU_SELF_USAGE] = (100.0f * (sys.QuadPart - m_lastSystemCPU.QuadPart) + (user.QuadPart - m_lastUserCPU.QuadPart))
    / (now.QuadPart - m_lastCPU.QuadPart) * m_numProcessors;

  m_lastCPU = now;
  m_lastUserCPU = user;
  m_lastSystemCPU = sys;
}
}