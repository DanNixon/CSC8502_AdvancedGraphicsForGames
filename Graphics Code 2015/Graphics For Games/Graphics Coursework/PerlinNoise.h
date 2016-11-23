/** @file */

#pragma once

#include <vector>

namespace GraphicsCoursework
{
/**
 * @class PerlinNoise
 * @brief 3D Perlin Noise generator.
 *
 * Adapted from: https://github.com/sol-prog/Perlin_Noise
 * Based on: http://mrl.nyu.edu/~perlin/noise/ (copyright 2002 Ken Perlin)
 */
class PerlinNoise
{
public:
  static const std::vector<int> REFERENCE_PERMUTATION;

public:
  PerlinNoise();
  PerlinNoise(unsigned int seed);
  virtual ~PerlinNoise();

  float Noise(float x, float y, float z) const;

protected:
  float Fade(float t) const;
  float Grad(int hash, float x, float y, float z) const;

protected:
  std::vector<int> m_permutation; //!< The permutation vector
};
}
