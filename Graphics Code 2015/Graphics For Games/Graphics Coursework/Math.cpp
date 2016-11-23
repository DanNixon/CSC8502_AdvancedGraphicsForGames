/** @file */

#include "Math.h"

namespace GraphicsCoursework
{
  /**
   * @brief Linearly interpolate between two points.
   * @param f Interpolation point
   * @param a Lower bound
   * @param b Upper bound
   * @return Interpolated value
   */
  float Math::Lerp(float f, float a, float b)
  {
    return a + f * (b - a);
  }
}