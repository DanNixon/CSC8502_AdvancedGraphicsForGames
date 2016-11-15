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