/** @file */

#include "MemoryMonitoring.h"

#include <psapi.h>

namespace GraphicsCoursework
{
void MemoryMonitoring::Init()
{
  m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&m_memInfo);
}

float MemoryMonitoring::TotalVirtualMemory()
{
  return (float)m_memInfo.ullTotalPageFile;
}

float MemoryMonitoring::SelfUsedVirtualMemory()
{
  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
  return (float)pmc.PagefileUsage;
}

float MemoryMonitoring::TotalPhysicalMemory()
{
  return (float)m_memInfo.ullTotalPhys;
}

float MemoryMonitoring::SelfUsedPhysicalMemory()
{
  PROCESS_MEMORY_COUNTERS pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
  return (float)pmc.WorkingSetSize;
}
}