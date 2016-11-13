#include "WindowsSystemMonitor.h"

#include <windows.h>
#include <psapi.h>

namespace GraphicsCoursework
{
  WindowsSystemMonitor::WindowsSystemMonitor()
  {
  }

  WindowsSystemMonitor::~WindowsSystemMonitor()
  {
  }

  void WindowsSystemMonitor::Update()
  {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    m_metrics[PHYSICAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPhys) / 1e6f;
    m_metrics[VIRTUAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPageFile) / 1e6f;

    m_metrics[PHYSICAL_MEMORY_USED] = ((float)(memInfo.ullTotalPhys - memInfo.ullAvailPhys)) / 1e6f;
    m_metrics[VIRTUAL_MEMORY_USED] = ((float)(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile)) / 1e6f;

    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

    m_metrics[PHYSICAL_MEMORY_SELF_USED] = ((float)pmc.WorkingSetSize) / 1e6f;
    m_metrics[PHYSICAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakWorkingSetSize) / 1e6f;

    m_metrics[VIRTUAL_MEMORY_SELF_USED] = ((float)pmc.PagefileUsage) / 1e6f;
    m_metrics[VIRTUAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakPagefileUsage) / 1e6f;
  }
}