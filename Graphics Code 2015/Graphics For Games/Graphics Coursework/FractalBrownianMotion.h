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

    float Fractal(size_t numOctaves, float x, float y, float z = 0.5f) const;

  protected:
    const PerlinNoise &m_perlin;

    float m_frequency;
    float m_amplitude;
    float m_lacunarity;
    float m_persistence;
  };
}