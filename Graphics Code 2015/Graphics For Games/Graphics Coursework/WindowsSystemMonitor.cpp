#include "WindowsSystemMonitor.h"

// clang-format off
#include <windows.h>
#include <psapi.h>
// clang-format on

namespace GraphicsCoursework
{
const float BYTES_TO_MB = 1.0f / 1e6f;

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

  m_metrics[PHYSICAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPhys) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_TOTAL] = ((float)memInfo.ullTotalPageFile) * BYTES_TO_MB;

  m_metrics[PHYSICAL_MEMORY_USED] = ((float)(memInfo.ullTotalPhys - memInfo.ullAvailPhys)) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_USED] =
      ((float)(memInfo.ullTotalPageFile - memInfo.ullAvailPageFile)) * BYTES_TO_MB;

  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

  m_metrics[PHYSICAL_MEMORY_SELF_USED] = ((float)pmc.WorkingSetSize) * BYTES_TO_MB;
  m_metrics[PHYSICAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakWorkingSetSize) * BYTES_TO_MB;

  m_metrics[VIRTUAL_MEMORY_SELF_USED] = ((float)pmc.PagefileUsage) * BYTES_TO_MB;
  m_metrics[VIRTUAL_MEMORY_SELF_USED_PEAK] = ((float)pmc.PeakPagefileUsage) * BYTES_TO_MB;
}
}