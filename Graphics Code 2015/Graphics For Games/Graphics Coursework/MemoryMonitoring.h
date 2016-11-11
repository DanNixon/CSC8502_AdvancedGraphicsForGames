/** @file */

#pragma once

#include <windows.h>

namespace GraphicsCoursework
{
class MemoryMonitoring
{
public:
  static void Init();

  static float TotalVirtualMemory();
  static float SelfUsedVirtualMemory();

  static float TotalPhysicalMemory();
  static float SelfUsedPhysicalMemory();

private:
  static MEMORYSTATUSEX m_memInfo;
};
}