/** @file */

#include "FractalBrownianMotion.h"

#include "PerlinNoise.h"

namespace GraphicsCoursework
{
  FractalBrownianMotion::FractalBrownianMotion(const PerlinNoise &perlin)
    : m_perlin(perlin)
    , m_numOctaves(1)
    , m_zValue(0.5f)
    , m_frequency(1.0f)
    , m_amplitude(1.0f)
    , m_lacunarity(2.0f)
    , m_persistence(0.5f)
{
}

FractalBrownianMotion::~FractalBrownianMotion()
{
}

float FractalBrownianMotion::Fractal(float x, float y) const
{
  float retVal = 0.0f;

  float denom = 0.0f;
  float frequency(m_frequency);
  float amplitude(m_amplitude);

  for (size_t i = 0; i < m_numOctaves; i++)
  {
    retVal += (amplitude * m_perlin.Noise(x * frequency, y * frequency, m_zValue * frequency));

    denom += amplitude;

    frequency *= m_lacunarity;
    amplitude *= m_persistence;
  }

  retVal /= denom;

  return retVal;
}
}