/** @file */

#pragma once

namespace GraphicsCoursework
{
class PerlinNoise;

class FractalBrownianMotion
{
public:
  FractalBrownianMotion(const PerlinNoise &perlin);
  virtual ~FractalBrownianMotion();

  float Fractal(float x, float y) const;

  size_t &NumOctaves()
  {
    return m_numOctaves;
  }

  float &ZValue()
  {
    return m_zValue;
  }

  float &Frequency()
  {
    return m_frequency;
  }

  float &Amplitude()
  {
    return m_amplitude;
  }

  float &Lacunarity()
  {
    return m_lacunarity;
  }

  float &Persistence()
  {
    return m_persistence;
  }

protected:
  const PerlinNoise &m_perlin;

  size_t m_numOctaves;
  float m_zValue;
  float m_frequency;
  float m_amplitude;
  float m_lacunarity;
  float m_persistence;
};
}