/** @file */

#include "FractalBrownianMotion.h"

#include "PerlinNoise.h"

namespace GraphicsCoursework
{
FractalBrownianMotion::FractalBrownianMotion(const PerlinNoise &perlin)
    : m_perlin(perlin)
    , m_frequency(1.0f)
    , m_amplitude(1.0f)
    , m_lacunarity(2.0f)
    , m_persistence(0.5f)
{
}

FractalBrownianMotion::~FractalBrownianMotion()
{
}

float FractalBrownianMotion::Fractal(size_t numOctaves, float x, float y, float z) const
{
  float retVal = 0.0f;

  float denom = 0.0f;
  float frequency(m_frequency);
  float amplitude(m_amplitude);

  for (size_t i = 0; i < numOctaves; i++)
  {
    retVal += (amplitude * m_perlin.Noise(x * frequency, y * frequency, z * frequency));

    denom += amplitude;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;
  }

  retVal /= denom;

  return retVal;
}
}